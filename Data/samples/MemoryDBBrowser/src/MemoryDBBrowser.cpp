//
// MemoryDBBrowser.cpp
//
// A web console for a Poco::Data::SQLite::MemoryDB, and a worked example of
// Poco::Data::SQLite::MemoryDBInspector.
//
// This process OWNS the store it serves - that is the only sound arrangement.
// A MemoryDB's data lives in its owner's memory, so a second process could see
// nothing but flushed shard files, and merely holding one of those open blocks
// the owner's atomic rename and defers its flushes. An inspector belongs to
// whoever owns the database; that is what this sample demonstrates, and what
// lets a service such as HubMonitor expose its own store the same way.
//
// The interesting part is not the HTTP: it is Store below, which is deliberately
// written to show MemoryDBInspector's three-group threading contract, since
// getting that wrong is the one way to misuse the class.
//
// The database directory comes from the command line (/db=DIR) or from
// MemoryDBBrowser.properties (memorydb.directory); everything else - bind
// address, port, webroot, row cap, write policy - is configurable the same way.
//
// HTTP is served by Poco::Net::HTTPReactorServer: idle keep-alive connections
// sit in a PollSet instead of pinning a thread, so several open browser tabs
// cost almost nothing. There is no dispatch thread pool - handlers run inline on
// the reactor worker thread that polled the socket readable - so several of them
// can be inside Store at once, which is exactly why Store has a mutex.
//
// Copyright (c) 2026, Aleph ONE Software Engineering LLC.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0


#include "Poco/Data/SQLite/MemoryDB.h"
#include "Poco/Data/SQLite/MemoryDBInspector.h"
#include "Poco/Net/HTTPReactorServer.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPRequestHandler.h"
#include "Poco/Net/HTTPRequestHandlerFactory.h"
#include "Poco/Net/HTTPServerParams.h"
#include "Poco/Net/HTTPServerRequest.h"
#include "Poco/Net/HTTPServerResponse.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/JSON/Object.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/ServerApplication.h"
#include "Poco/Exception.h"
#include "Poco/File.h"
#include "Poco/JSONString.h"
#include "Poco/Mutex.h"
#include "Poco/Path.h"
#include "Poco/StreamCopier.h"
#include "Poco/String.h"
#include "Poco/URI.h"
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>


using Poco::Data::SQLite::MemoryDB;
using Poco::Data::SQLite::MemoryDBInspector;


namespace
{


std::string mimeType(const std::string& path)
{
	const auto pos = path.find_last_of('.');
	if (pos == std::string::npos) return "application/octet-stream";
	static const std::unordered_map<std::string, std::string> map = {
		{".html", "text/html; charset=utf-8"},
		{".js",   "application/javascript; charset=utf-8"},
		{".css",  "text/css; charset=utf-8"},
		{".json", "application/json; charset=utf-8"},
		{".svg",  "image/svg+xml"},
		{".png",  "image/png"},
		{".ico",  "image/x-icon"},
		{".txt",  "text/plain; charset=utf-8"},
	};
	const auto it = map.find(path.substr(pos));
	return it != map.end() ? it->second : std::string("application/octet-stream");
}


void sendJSON(Poco::Net::HTTPServerResponse& response, const std::string& body,
	Poco::Net::HTTPResponse::HTTPStatus status = Poco::Net::HTTPResponse::HTTP_OK)
{
	response.setStatus(status);
	response.setContentType("application/json; charset=utf-8");
	response.set("Cache-Control", "no-store");
	response.setContentLength(static_cast<std::streamsize>(body.size()));
	response.send().write(body.data(), static_cast<std::streamsize>(body.size()));
}


void sendError(Poco::Net::HTTPServerResponse& response, const std::string& message,
	Poco::Net::HTTPResponse::HTTPStatus status)
{
	std::ostringstream os;
	os << "{\"ok\":false,\"error\":";
	Poco::toJSON(message, os, Poco::JSON_WRAP_STRINGS);
	os << '}';
	sendJSON(response, os.str(), status);
}


class Store
	/// Owns the MemoryDB and an inspector over it, and shows how an owner is meant
	/// to honour MemoryDBInspector's threading contract. Each method below is
	/// annotated with the group it belongs to; the differences between them are
	/// the whole point of this class.
{
public:
	Store(const std::string& dir, const MemoryDB::Options& options,
		MemoryDBInspector::SQLPolicy policy, std::size_t maxRows):
		_db(dir, options),
		_inspector(_db, policy)
	{
		_inspector.setMaxRows(maxRows);
	}

	std::string statusJSON()
		/// Group A: no lock at all. status() runs no SQL - it reads MemoryDB's own
		/// thread-safe accessors and walks the directory - so a browser polling
		/// this never contends with a writer.
	{
		std::ostringstream os;
		_inspector.writeStatusJSON(os);
		return os.str();
	}

	std::string schemaJSON()
		/// Group B: gather under the lock, serialize outside it.
	{
		MemoryDBInspector::Schema schema;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			schema = _inspector.schema(MemoryDBInspector::RowCounts::EXACT);
		}
		std::ostringstream os;
		MemoryDBInspector::writeJSON(os, schema);
		return os.str();
	}

	std::string queryJSON(const std::string& sql)
		/// Group B, and the reason the API hands back a struct rather than a
		/// string: formatting a 500-row grid is pure string work and has no
		/// business happening while a writer waits for the lock.
	{
		MemoryDBInspector::QueryResult result;
		{
			Poco::FastMutex::ScopedLock lock(_mutex);
			result = _inspector.query(sql);
		}
		std::ostringstream os;
		MemoryDBInspector::writeJSON(os, result);
		return os.str();
	}

	std::string actionJSON(const std::string& action, Poco::UInt32 shardId,
		const std::string& table)
		/// Group C: deliberately NOT under _mutex. perform("flush") holds
		/// MemoryDB's _flushMutex across the whole of the file IO, so taking the
		/// session lock here would stall every reader and writer for the duration.
	{
		std::ostringstream os;
		_inspector.writeActionJSON(os, action, shardId, table);
		return os.str();
	}

private:
	Poco::FastMutex   _mutex;   // serialises Session access, as any owner must
	MemoryDB          _db;
	MemoryDBInspector _inspector;
};


class ApiHandler: public Poco::Net::HTTPRequestHandler
	/// GET /api/status, GET /api/schema, POST /api/query, POST /api/action.
{
public:
	ApiHandler(Store& store, std::string path): _store(store), _path(std::move(path)) {}

	void handleRequest(Poco::Net::HTTPServerRequest& request,
		Poco::Net::HTTPServerResponse& response) override
	{
		try
		{
			if (_path == "/api/status") { sendJSON(response, _store.statusJSON()); return; }
			if (_path == "/api/schema") { sendJSON(response, _store.schemaJSON()); return; }

			if (request.getMethod() != Poco::Net::HTTPRequest::HTTP_POST)
			{
				sendError(response, "POST required", Poco::Net::HTTPResponse::HTTP_METHOD_NOT_ALLOWED);
				return;
			}

			// Poco::JSON is still the right tool for PARSING a request body. What
			// moved into the library is JSON production, which is why this sample
			// no longer builds a single response object by hand.
			Poco::JSON::Parser parser;
			Poco::JSON::Object::Ptr body =
				parser.parse(request.stream()).extract<Poco::JSON::Object::Ptr>();

			if (_path == "/api/query")
			{
				const std::string sql = Poco::trim(body->optValue<std::string>("sql", ""));
				if (sql.empty())
				{
					sendError(response, "empty sql", Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
					return;
				}
				sendJSON(response, _store.queryJSON(sql));
				return;
			}

			if (_path == "/api/action")
			{
				sendJSON(response, _store.actionJSON(
					body->optValue<std::string>("action", ""),
					static_cast<Poco::UInt32>(body->optValue<Poco::UInt64>("shardId", 0)),
					body->optValue<std::string>("table", "")));
				return;
			}

			sendError(response, "not found", Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
		}
		catch (const Poco::Exception& exc)
		{
			sendError(response, exc.displayText(), Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
		}
	}

private:
	Store&      _store;
	std::string _path;
};


class StaticFileHandler: public Poco::Net::HTTPRequestHandler
	/// Serves the web UI out of the webroot directory. Files are re-read per
	/// request and sent with Cache-Control: no-store, so editing the UI on a
	/// deployed box takes effect on reload.
{
public:
	StaticFileHandler(std::string webroot, std::string relpath):
		_webroot(std::move(webroot)), _relpath(std::move(relpath)) {}

	void handleRequest(Poco::Net::HTTPServerRequest&,
		Poco::Net::HTTPServerResponse& response) override
	{
		if (_relpath.empty() || _relpath == "/") _relpath = "/index.html";
		if (_relpath.find("..") != std::string::npos)
		{
			response.setStatus(Poco::Net::HTTPResponse::HTTP_BAD_REQUEST);
			response.setContentLength(0);
			response.send();
			return;
		}

		Poco::Path path(_webroot);
		path.append(Poco::Path(_relpath, Poco::Path::PATH_UNIX));
		const std::string full = path.toString();

		Poco::File file(full);
		std::ifstream in(full.c_str(), std::ios::binary);
		if (!file.exists() || !file.isFile() || !in)
		{
			response.setStatus(Poco::Net::HTTPResponse::HTTP_NOT_FOUND);
			response.setContentType("text/plain; charset=utf-8");
			const std::string body = "Not found: " + _relpath +
				" (webroot: " + (_webroot.empty() ? "<unset>" : _webroot) + ")";
			response.setContentLength(static_cast<std::streamsize>(body.size()));
			response.send().write(body.data(), static_cast<std::streamsize>(body.size()));
			return;
		}

		response.setStatus(Poco::Net::HTTPResponse::HTTP_OK);
		response.setContentType(mimeType(full));
		response.setContentLength(static_cast<std::streamsize>(file.getSize()));
		response.set("Cache-Control", "no-store");
		Poco::StreamCopier::copyStream(in, response.send());
	}

private:
	std::string _webroot;
	std::string _relpath;
};


class HandlerFactory: public Poco::Net::HTTPRequestHandlerFactory
{
public:
	HandlerFactory(Store& store, std::string webroot):
		_store(store), _webroot(std::move(webroot)) {}

	Poco::Net::HTTPRequestHandler* createRequestHandler(
		const Poco::Net::HTTPServerRequest& request) override
	{
		Poco::URI uri;
		try { uri = Poco::URI(request.getURI()); } catch (...) { uri = Poco::URI("/"); }
		const std::string& path = uri.getPath();

		if (path.rfind("/api/", 0) == 0) return new ApiHandler(_store, path);
		return new StaticFileHandler(_webroot, path == "/" ? "/index.html" : path);
	}

private:
	Store&      _store;
	std::string _webroot;
};


} // namespace


class MemoryDBBrowser: public Poco::Util::ServerApplication
{
public:
	MemoryDBBrowser(): _helpRequested(false) {}

protected:
	void initialize(Application& self) override
	{
		loadConfiguration();
		ServerApplication::initialize(self);
	}

	void defineOptions(Poco::Util::OptionSet& options) override
	{
		ServerApplication::defineOptions(options);

		options.addOption(Poco::Util::Option("help", "h",
			"display help information and exit")
			.required(false).repeatable(false));

		options.addOption(Poco::Util::Option("db", "d",
			"MemoryDB persistence directory (created if missing). This process opens "
			"it for writing, so nothing else may have it open")
			.required(false).repeatable(false)
			.argument("DIR").binding("memorydb.directory"));

		options.addOption(Poco::Util::Option("allow-writes", "W",
			"let the SQL console run statements other than SELECT. Off by default")
			.required(false).repeatable(false));

		options.addOption(Poco::Util::Option("bind", "b",
			"address to listen on (default 127.0.0.1)")
			.required(false).repeatable(false)
			.argument("ADDRESS").binding("memorydb.http.bind"));

		options.addOption(Poco::Util::Option("port", "p",
			"port to listen on (default 9880)")
			.required(false).repeatable(false)
			.argument("PORT").binding("memorydb.http.port"));

		options.addOption(Poco::Util::Option("webroot", "w",
			"directory holding the web UI (default <exe dir>/webui)")
			.required(false).repeatable(false)
			.argument("DIR").binding("memorydb.webroot"));

		options.addOption(Poco::Util::Option("max-rows", "m",
			"maximum result rows sent to the browser (default 500)")
			.required(false).repeatable(false)
			.argument("N").binding("memorydb.query.maxRows"));

		options.addOption(Poco::Util::Option("load-archived", "a",
			"merge every sealed shard into RAM on open, so plain SELECTs see all history")
			.required(false).repeatable(false));
	}

	void handleOption(const std::string& name, const std::string& value) override
	{
		ServerApplication::handleOption(name, value);

		if (name == "help")
		{
			_helpRequested = true;
			stopOptionsProcessing();
		}
		else if (name == "load-archived") config().setBool("memorydb.loadArchivedShards", true);
		else if (name == "allow-writes")   config().setBool("memorydb.allowWrites", true);
	}

	int main(const std::vector<std::string>& args) override
	{
		if (_helpRequested)
		{
			displayHelp();
			return Application::EXIT_OK;
		}

		// A bare positional argument is the database directory, so
		// `MemoryDBBrowser /var/lib/myapp/db` works without any option syntax.
		std::string dir = config().getString("memorydb.directory", "");
		if (dir.empty() && !args.empty()) dir = args[0];
		if (dir.empty())
		{
			logger().error("No database directory. Pass one as an argument, as "
				"--db=DIR, or set memorydb.directory in MemoryDBBrowser.properties.");
			displayHelp();
			return Application::EXIT_USAGE;
		}

		const bool allowWrites = config().getBool("memorydb.allowWrites", false);
		const std::string bind = config().getString("memorydb.http.bind", "127.0.0.1");
		const int port = config().getInt("memorydb.http.port", 9880);
		const std::string webroot = resolveWebroot();
		const std::size_t maxRows =
			static_cast<std::size_t>(config().getUInt64("memorydb.query.maxRows", 500));

		MemoryDB::Options options;
		options.loadArchivedShards = config().getBool("memorydb.loadArchivedShards", false);
		options.idleInterval = Poco::Timespan(config().getInt("memorydb.idleIntervalSeconds", 30), 0);
		options.shardMaxBytes = config().getUInt64("memorydb.shardMaxBytes", options.shardMaxBytes);

		try
		{
			Store store(dir, options,
				allowWrites ? MemoryDBInspector::SQLPolicy::ALLOW_WRITES
				            : MemoryDBInspector::SQLPolicy::READ_ONLY,
				maxRows);

			Poco::Net::HTTPServerParams::Ptr params = new Poco::Net::HTTPServerParams;
			// TCPReactorServer asserts on getAcceptorNum() unless reactor mode is
			// on, so these two are mandatory, not tuning.
			params->setReactorMode(true);
			params->setAcceptorNum(1);
			params->setKeepAlive(true);
			params->setKeepAliveTimeout(Poco::Timespan(15, 0));
			params->setServerName("MemoryDBBrowser");
			params->setMaxThreads(4);
			// Handlers write the response inline on the reactor thread over a
			// blocking socket; a client that stops reading would otherwise wedge
			// that reactor and every connection assigned to it.
			params->setSendTimeout(Poco::Timespan(30, 0));

			Poco::Net::HTTPReactorServer server(
				Poco::Net::SocketAddress(bind, static_cast<Poco::UInt16>(port)),
				params, new HandlerFactory(store, webroot));
			server.start();

			logger().information("MemoryDB directory: %s (SQL console is %s)", dir,
				std::string(allowWrites ? "read-write" : "read-only"));
			logger().information("Web UI on http://%s:%d/ (webroot=%s)", bind, port, webroot);
			if (!Poco::File(webroot).exists())
			{
				logger().warning("Webroot %s does not exist - the UI will 404. "
					"Copy the webui directory next to the executable or pass /webroot=DIR.",
					webroot);
			}
			std::cout << "MemoryDBBrowser listening on http://" << bind << ':' << port
				<< "/  (database: " << dir << ")" << std::endl;

			waitForTerminationRequest();

			server.stop();
		}
		catch (const Poco::Exception& exc)
		{
			logger().error("MemoryDBBrowser failed: %s", exc.displayText());
			std::cerr << exc.displayText() << std::endl;
			return Application::EXIT_SOFTWARE;
		}

		return Application::EXIT_OK;
	}

private:
	std::string resolveWebroot() const
		/// Configured value wins, then webui/ next to the executable (what CMake
		/// stages, and what a deployment copies alongside the binary), then webui/
		/// under the working directory.
	{
		const std::string configured = config().getString("memorydb.webroot", "");
		if (!configured.empty()) return configured;

		Poco::Path beside(config().getString("application.dir"));
		beside.pushDirectory("webui");
		if (Poco::File(beside.toString()).exists()) return beside.toString();

		return Poco::Path::current() + "webui";
	}

	void displayHelp() const
	{
		Poco::Util::HelpFormatter formatter(options());
		formatter.setCommand(commandName());
		formatter.setUsage("[OPTIONS] [DIRECTORY]");
		formatter.setHeader("A web console for a Poco::Data::SQLite::MemoryDB store, "
			"built on MemoryDBInspector: browse its shard files and schema, and run SQL "
			"against it. This process owns the store, so nothing else may have the "
			"directory open.");
		formatter.format(std::cout);
	}

	bool _helpRequested;
};


POCO_SERVER_MAIN(MemoryDBBrowser)
