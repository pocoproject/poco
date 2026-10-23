//
// ModulesTest.cpp
//
// Copyright (c) 2025, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "ModulesTest.h"
#include "CppUnit/TestCaller.h"
#include "CppUnit/TestSuite.h"
#include "Poco/Bugcheck.h"
#include <sstream>
#include <string>
#include <type_traits>
#include <vector>


// Apart from the poco_assert macro of Poco/Bugcheck.h, every POCO name below must come
// through the import, so that a name a partition does not export fails the build.
import Poco;


namespace {


void incrementCounter(int& counter)
{
	++counter;
}


} // namespace


ModulesTest::ModulesTest(const std::string& name): CppUnit::TestCase(name)
{
}


ModulesTest::~ModulesTest()
{
}


void ModulesTest::testFoundation()
{
	poco_assert(true);

	const Poco::URI uri("http://www.appinf.com:81/sample?example-query#somewhere");
	assertEqual("http", uri.getScheme());
	assertEqual("www.appinf.com:81", uri.getAuthority());
	assertEqual("/sample", uri.getPath());
	assertEqual("example-query", uri.getQuery());
	assertEqual("somewhere", uri.getFragment());

	const Poco::LocalDateTime now;
	const std::string formatted = Poco::DateTimeFormatter::format(now, Poco::DateTimeFormat::ISO8601_FORMAT);
	Poco::DateTime parsed;
	int tzd = 0;
	Poco::DateTimeParser::parse(Poco::DateTimeFormat::ISO8601_FORMAT, formatted, parsed, tzd);
	parsed.makeUTC(tzd);
	const Poco::LocalDateTime local(tzd, parsed);
	assertEqual(formatted, Poco::DateTimeFormatter::format(local, Poco::DateTimeFormat::ISO8601_FORMAT));

	const Poco::Dynamic::Var three(3);
	const int sum = 2 + three;
	assertEqual(5, sum);
	assertTrue(3 == three);
	const Poco::Dynamic::Var dateTime(Poco::DateTime(2025, 1, 2, 3, 4, 5));
	assertEqual("2025-01-02T03:04:05Z", dateTime.convert<std::string>());

	assertEqual("a-1", Poco::format("%s-%d", std::string("a"), 1));
	assertEqual("ab", Poco::cat(std::string("a"), std::string("b")));

	std::ostringstream base64;
	Poco::Base64Encoder encoder(base64, Poco::BASE64_URL_ENCODING);
	encoder << "!@#$%^&*()_~<>";
	encoder.close();
	assertEqual("IUAjJCVeJiooKV9-PD4=", base64.str());

	Poco::ULIDGenerator generator;
	const Poco::ULID ulid = generator.create();
	Poco::ULID parsedULID;
	parsedULID.parse(ulid.toString());
	assertTrue(parsedULID == ulid);

	Poco::OrderedMap<std::string, int> orderedMap;
	orderedMap.insert({"z", 1});
	orderedMap.insert({"a", 2});
	orderedMap.insert({"m", 3});
	std::string keys;
	for (const auto& entry: orderedMap) keys += entry.first;
	assertEqual("zam", keys);

	Poco::SPSCQueue<int> queue(4);
	assertTrue(queue.tryPush(42));
	int popped = 0;
	assertTrue(queue.tryPop(popped));
	assertEqual(42, popped);
	assertFalse(queue.tryPop(popped));

	Poco::BasicEvent<int> event;
	event += Poco::priorityDelegate(&incrementCounter, 1);
	int counter = 0;
	event.notify(this, counter);
	assertEqual(1, counter);
	event -= Poco::priorityDelegate(&incrementCounter, 1);
	event.notify(this, counter);
	assertEqual(1, counter);

	char buffer[POCO_MAX_INT_STRING_LEN];
	std::size_t size = POCO_MAX_INT_STRING_LEN;
	assertTrue(Poco::intToStr(-1234, 10, buffer, size));
	assertEqual("-1234", std::string(buffer, size));

	static_assert(Poco::PROCESS_EXIT_EXEC_FAILED == 72);
	static_assert(Poco::PROCESS_CLOSE_STDIN == 1);

#ifdef POCO_ENABLE_FASTLOGGER
	static_assert(std::is_class_v<Poco::FastLogger>);
#endif
}


void ModulesTest::testEncodings()
{
#ifdef ENABLE_ENCODINGS
	Poco::registerExtraEncodings();
	const Poco::TextEncoding::Ptr pEncoding = Poco::TextEncoding::find("windows-1258");
	assertTrue(pEncoding != nullptr);
	assertEqual("windows-1258", std::string(pEncoding->canonicalName()));
#endif
}


void ModulesTest::testXML()
{
#ifdef ENABLE_XML
	Poco::XML::DOMParser parser;
	const Poco::XML::AutoPtr<Poco::XML::Document> pDocument = parser.parseString("<root attr=\"1\"><child/></root>");
	const Poco::XML::Element* pRoot = pDocument->documentElement();
	assertTrue(pRoot != nullptr);
	const Poco::XML::XMLString name = pRoot->nodeName();
	assertEqual("root", name);
	assertEqual("1", pRoot->getAttribute("attr"));

	const Poco::XML::QName qname("urn:ns", "root");
	std::ostringstream ostr;
	ostr << qname;
	assertEqual("urn:ns#root", ostr.str());
#endif
}


void ModulesTest::testJSON()
{
#ifdef ENABLE_JSON
	Poco::JSON::Parser parser;
	const Poco::Dynamic::Var result = parser.parse("{\"name\":\"poco\",\"version\":2}");
	const Poco::JSON::Object::Ptr& pObject = result.extract<Poco::JSON::Object::Ptr>();
	assertTrue(pObject != nullptr);
	assertEqual("poco", pObject->getValue<std::string>("name"));
	assertTrue(2 == pObject->get("version"));

	const Poco::Dynamic::Var object(pObject);
	assertEqual("{\"name\":\"poco\",\"version\":2}", object.convert<std::string>());
#endif
}


void ModulesTest::testUtil()
{
#ifdef ENABLE_UTIL
	Poco::AutoPtr<Poco::Util::MapConfiguration> pMapConfig = new Poco::Util::MapConfiguration;
	pMapConfig->setString("prop1", "value1");
	pMapConfig->setInt("prop2", 42);
	assertEqual("value1", pMapConfig->getString("prop1"));
	assertEqual(42, pMapConfig->getInt("prop2"));

#ifdef ENABLE_XML
	std::istringstream xml("<config><prop1>value1</prop1><prop2><prop3 attr=\"x\">value3</prop3></prop2></config>");
	const Poco::AutoPtr<Poco::Util::XMLConfiguration> pXMLConfig = new Poco::Util::XMLConfiguration(xml);
	assertEqual("value1", pXMLConfig->getString("prop1"));
	assertEqual("value3", pXMLConfig->getString("prop2.prop3"));
	assertEqual("x", pXMLConfig->getString("prop2.prop3[@attr]"));
#endif

	const Poco::Util::Units::Values::km distance(Poco::Util::Units::Values::m(1500));
	assertEqualDelta(1.5, distance.get(), 1e-12);
	std::ostringstream ostr;
	ostr << Poco::Util::Units::Values::m(2.5);
	assertEqual("2.5 m", ostr.str());
	const Poco::Util::Units::Values::m astronomicalUnit(Poco::Util::Units::Constants::AU);
	assertEqualDelta(149597871000.0, astronomicalUnit.get(), 1.0);
#endif
}


void ModulesTest::testNet()
{
#ifdef ENABLE_NET
	const Poco::Net::IPAddress address("192.168.1.120");
	std::ostringstream addressStream;
	addressStream << address;
	assertEqual("192.168.1.120", addressStream.str());

	const Poco::Net::SocketAddress socketAddress("192.168.1.100", 100);
	assertEqual(100, socketAddress.port());
	assertEqual("192.168.1.100:100", socketAddress.toString());

	const Poco::Net::HTTPRequest request(Poco::Net::HTTPRequest::HTTP_POST, "/index.html", Poco::Net::HTTPMessage::HTTP_1_1);
	assertEqual("POST", request.getMethod());
	assertEqual("/index.html", request.getURI());

	const Poco::Net::ProxyConfig proxyConfig;
	assertTrue(proxyConfig.host.empty());
	assertEqual(80, proxyConfig.port);
	assertEqual("http", proxyConfig.protocol);

#if defined(__linux__) || defined(__APPLE__) || defined(_WIN32)
	const Poco::Net::NetworkInterface::MACAddress mac{0x00, 0x1a, 0x2b, 0x3c, 0x4d, 0x5e};
	std::ostringstream macStream;
	macStream << mac;
	const std::string separator(1, Poco::Net::NetworkInterface::MAC_SEPARATOR);
	assertEqual("00" + separator + "1a" + separator + "2b" + separator + "3c" + separator + "4d" + separator + "5e", macStream.str());
#endif
#endif
}


void ModulesTest::testNetSSL()
{
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	Poco::Net::initializeSSL();
	Poco::Net::uninitializeSSL();
	static_assert(std::is_class_v<Poco::Net::Context>);
#ifdef ENABLE_NETSSL_OPENSSL
	static_assert(std::is_class_v<Poco::Net::FTPSClientSession>);
#endif
#endif
}


void ModulesTest::testCrypto()
{
#ifdef ENABLE_CRYPTO
	Poco::Crypto::DigestEngine engine("SHA256");
	engine.update("abc");
	assertEqual("ba7816bf8f01cfea414140de5dae2223b00361a396177a9cb410ff61f20015ad", Poco::DigestEngine::digestToHex(engine.digest()));

	Poco::Crypto::Cipher::Ptr pCipher = Poco::Crypto::CipherFactory::defaultFactory().createCipher(
		Poco::Crypto::RSAKey(Poco::Crypto::RSAKey::KL_1024, Poco::Crypto::RSAKey::EXP_SMALL),
		RSA_PADDING_PKCS1_OAEP);
	const std::string plain("modules");
	assertEqual(plain, pCipher->decryptString(pCipher->encryptString(plain)));
#endif
}


void ModulesTest::testJWT()
{
#ifdef ENABLE_JWT
	Poco::JWT::Token token;
	token.setType("JWT");
	token.setSubject("1234567890");
	token.payload().set("name", std::string("John Doe"));
	token.setIssuedAt(Poco::Timestamp::fromEpochTime(1516239022));

	Poco::JWT::Signer signer("0123456789ABCDEF0123456789ABCDEF");
	const std::string jwt = signer.sign(token, Poco::JWT::Signer::ALGO_HS256);
	assertEqual("eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpYXQiOjE1MTYyMzkwMjIsIm5hbWUiOiJKb2huIERvZSIsInN1YiI6IjEyMzQ1Njc4OTAifQ.qn9G7NwFEOjIh-7hfCUDZA1aJeQmf7I7YvzCBcdenGw", jwt);

	const Poco::JWT::Token verified = signer.verify(jwt);
	assertEqual("HS256", verified.getAlgorithm());
	assertEqual("1234567890", verified.getSubject());
	assertEqual("John Doe", verified.payload().getValue<std::string>("name"));
#endif
}


void ModulesTest::testData()
{
#ifdef ENABLE_DATA
	const Poco::Data::Date date(2007, 6, 15);
	const Poco::Dynamic::Var dateVar(date);
	assertTrue(dateVar.isDate());
	assertEqual("2007/06/15", dateVar.convert<std::string>());
	Poco::Data::Date copy;
	copy = dateVar;
	assertTrue(copy == date);

	Poco::Data::CLOB first(std::string("abc"));
	Poco::Data::CLOB second(std::string("de"));
	swap(first, second);
	assertEqual(2, first.size());
	assertEqual(3, second.size());
#endif
}


void ModulesTest::testSQLite()
{
#ifdef ENABLE_DATA_SQLITE
	using namespace Poco::Data::Keywords;

	Poco::Data::SQLite::Connector::registerConnector();
	{
		Poco::Data::Session session("SQLite", ":memory:");
		session << "CREATE TABLE Person (Name VARCHAR(30), Age INTEGER, Nick VARCHAR(30))", now;

		std::string name("Bart");
		int age = 10;
		session << "INSERT INTO Person VALUES (?, ?, ?)", use(name), use(age), bind(null), now;

		std::string resultName;
		int resultAge = 0;
		Poco::Nullable<std::string> resultNick("placeholder");
		session << "SELECT Name, Age, Nick FROM Person", into(resultName), into(resultAge), into(resultNick), now;
		assertEqual("Bart", resultName);
		assertEqual(10, resultAge);
		assertTrue(resultNick.isNull());
	}
	Poco::Data::SQLite::Connector::unregisterConnector();
#endif
}


void ModulesTest::testODBC()
{
#ifdef ENABLE_DATA_ODBC
	static_assert(POCO_ODBC_NULL_HANDLE == nullptr);
	static_assert(std::is_class_v<Poco::Data::ODBC::Connector>);
#endif
}


void ModulesTest::testMongoDB()
{
#ifdef ENABLE_MONGODB
	Poco::MongoDB::Document document;
	const Poco::MongoDB::Decimal128::Ptr pDecimal = new Poco::MongoDB::Decimal128(Poco::MongoDB::Decimal128::fromString("1.5"));
	document.add(std::string("decimal"), pDecimal);
	document.add(std::string("min"), Poco::MongoDB::MinKey{});
	document.add(std::string("max"), Poco::MongoDB::MaxKey{});
	assertTrue(document.isType<Poco::MongoDB::MinKey>("min"));
	assertEqual("{\"decimal\":{\"$numberDecimal\": \"1.5\"},\"min\":{\"$minKey\": 1},\"max\":{\"$maxKey\": 1}}", document.toString());
#endif
}


void ModulesTest::testRedis()
{
#ifdef ENABLE_REDIS
	const Poco::Redis::Command command = Poco::Redis::Command::set("mykey", "Hello");
	assertEqual("*3\r\n$3\r\nSET\r\n$5\r\nmykey\r\n$5\r\nHello\r\n", command.toString());
	static_assert(std::is_base_of_v<Poco::Redis::RedisNotification, Poco::Redis::RedisConnectNotification>);
#endif
}


void ModulesTest::testPrometheus()
{
#ifdef ENABLE_PROMETHEUS
	Poco::Prometheus::Registry registry;
	Poco::Prometheus::Counter counter("requests_total", &registry);
	counter.help("Number of requests");
	counter.inc();
	counter.inc(2);

	std::ostringstream stream;
	Poco::Prometheus::TextExporter exporter(stream);
	registry.exportTo(exporter);
	assertEqual(
		"# HELP requests_total Number of requests\n"
		"# TYPE requests_total counter\n"
		"requests_total 3\n",
		stream.str());
#endif
}


void ModulesTest::testZip()
{
#ifdef ENABLE_ZIP
	const std::string content("Hello, modules!");
	std::stringstream archive(std::ios::in | std::ios::out | std::ios::binary);
	Poco::Zip::Compress compress(archive, true);
	std::istringstream input(content);
	compress.addFile(input, Poco::DateTime(2025, 1, 1), Poco::Path("hello.txt"), Poco::Zip::ZipCommon::CM_DEFLATE, Poco::Zip::ZipCommon::CL_MAXIMUM);
	const Poco::Zip::ZipArchive written = compress.close();
	assertTrue(written.findHeader("hello.txt") != written.headerEnd());

	archive.seekg(0);
	const Poco::Zip::ZipArchive readBack(archive);
	const auto itHeader = readBack.findHeader("hello.txt");
	assertTrue(itHeader != readBack.headerEnd());
	archive.clear();
	Poco::Zip::ZipInputStream zipInput(archive, itHeader->second);
	std::string result;
	Poco::StreamCopier::copyToString(zipInput, result);
	assertEqual(content, result);
#endif
}


void ModulesTest::testPDF()
{
#ifdef ENABLE_PDF
	Poco::PDF::Document document(1);
	const Poco::PDF::Page& page = document[0];
	page.setLineCap(Poco::PDF::HPDF_ROUND_END);
	assertTrue(page.getLineCap() == Poco::PDF::HPDF_ROUND_END);
	assertTrue(document.size() > 0);
#endif
}


void ModulesTest::testDNSSD()
{
#ifdef ENABLE_DNSSD
	const Poco::DNSSD::BrowseHandle handle;
	assertTrue(handle.isNull());
	static_assert(static_cast<int>(Poco::DNSSD::BrowseHandle::TYPE) == Poco::DNSSD::SD_BROWSE_HANDLE);
	static_assert(static_cast<int>(Poco::DNSSD::ServiceHandle::TYPE) == Poco::DNSSD::SD_SERVICE_HANDLE);
	static_assert(static_cast<int>(Poco::DNSSD::RecordHandle::TYPE) == Poco::DNSSD::SD_RECORD_HANDLE);
#endif
}


void ModulesTest::testCppParser()
{
#ifdef ENABLE_CPPPARSER
	assertEqual("func", Poco::CppParser::Symbol::extractName("void func(int arg1, int arg2)"));
#endif
}


void ModulesTest::testSevenZip()
{
#ifdef ENABLE_SEVENZIP
	const Poco::SevenZip::ArchiveEntry entry;
	assertTrue(entry.path().empty());
	assertEqual(0, entry.size());
#endif
}


void ModulesTest::testActiveRecord()
{
#ifdef ENABLE_ACTIVERECORD
	Poco::ActiveRecord::PostgresStatementPlaceholderProvider postgres;
	assertEqual("$1", postgres.next());
	assertEqual("$2", postgres.next());
	Poco::ActiveRecord::DefaultStatementPlaceholderProvider standard;
	assertEqual("?", standard.next());
#endif
}


void ModulesTest::testSSH()
{
#ifdef ENABLE_SSH
	Poco::TemporaryFile keyDir;
	keyDir.createDirectories();
	const Poco::File keyFile(Poco::SSH::SSHHostKeyManager::ensureHostKey(keyDir.path()));
	assertTrue(keyFile.exists());
	assertTrue(keyFile.getSize() > 0);
	static_assert(std::is_pointer_v<ssh_session>);
#endif
}


void ModulesTest::testCodeGeneration()
{
#ifdef ENABLE_CODEGENERATION
	assertEqual("MyLib_Foo_INCLUDED", Poco::CodeGeneration::Utility::createIncludeGuard("MyLib", "Foo"));
#endif
}


void ModulesTest::testRemotingNG()
{
#ifdef ENABLE_REMOTINGNG
	assertTrue(Poco::RemotingNG::URIUtility::matchPath("/api/user/123", "/api/user/{id}"));
	assertFalse(Poco::RemotingNG::URIUtility::matchPath("/api/user", "/api/user/{id}"));

	Poco::RemotingNG::EventFilter<int>::Ptr pFilter = new Poco::RemotingNG::IsGreaterThanFilter<int>(100);
	assertTrue(pFilter->accept(101));
	assertFalse(pFilter->accept(100));

	const std::vector<int> values{1, 2, 3};
	std::stringstream stream;
	Poco::RemotingNG::BinarySerializer serializer;
	serializer.setup(stream);
	serializer.serializeMessageBegin("message", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeSerializer<std::vector<int>>::serialize("values", values, serializer);
	serializer.serializeMessageEnd("message", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);

	Poco::RemotingNG::BinaryDeserializer deserializer;
	deserializer.setup(stream);
	std::vector<int> deserialized;
	deserializer.deserializeMessageBegin("message", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	Poco::RemotingNG::TypeDeserializer<std::vector<int>>::deserialize("values", true, deserializer, deserialized);
	deserializer.deserializeMessageEnd("message", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assertTrue(deserialized == values);

#ifdef ENABLE_REMOTINGNG_REST
	Poco::Net::HTTPRequest request;
	Poco::RemotingNG::REST::PathSerializer pathSerializer("/api/object/{id}/child/{childId}", request);
	pathSerializer.serialize("id", 1234);
	pathSerializer.serialize("childId", std::string("foo"));
	pathSerializer.close();
	assertEqual("/api/object/1234/child/foo", request.getURI());
#endif

#ifdef ENABLE_REMOTINGNG_SOAP
	Poco::RemotingNG::SOAP::Serializer soapSerializer;
	std::ostringstream soap;
	soapSerializer.setup(soap);
	soapSerializer.pushProperty(Poco::RemotingNG::Serializer::PROP_NAMESPACE, std::string("urn:schemas-appinf-com:service:MyService:1.0"));
	soapSerializer.serializeMessageBegin("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	soapSerializer.serializeMessageEnd("method", Poco::RemotingNG::SerializerBase::MESSAGE_REQUEST);
	assertEqual(
		"<soap:Envelope encodingStyle=\"http://schemas.xmlsoap.org/soap/encoding/\" xmlns:soap=\"http://schemas.xmlsoap.org/soap/envelope/\" xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\">"
		"<soap:Body>"
		"<ns1:method xmlns:ns1=\"urn:schemas-appinf-com:service:MyService:1.0\"/>"
		"</soap:Body>"
		"</soap:Envelope>",
		soap.str());
#endif

#ifdef ENABLE_REMOTINGNG_JSONRPC
	std::istringstream json("42");
	Poco::RemotingNG::JSONRPC::JSONParser jsonParser(json);
	const Poco::RemotingNG::JSONRPC::JSValue::Ptr pValue = jsonParser.parse();
	assertTrue(pValue != nullptr);
	assertEqual(42, pValue->asInt32());
#endif

#ifdef ENABLE_REMOTINGNG_TCP
	static_assert(std::is_base_of_v<Poco::RemotingNG::TransportFactory, Poco::RemotingNG::TCP::TransportFactory>);
#endif

#ifdef ENABLE_REMOTINGNG_HTTP
	static_assert(std::is_base_of_v<Poco::RemotingNG::TransportFactory, Poco::RemotingNG::HTTP::TransportFactory>);
#endif
#endif
}


void ModulesTest::testXSD()
{
#ifdef ENABLE_XSD_TYPES
	Poco::XSD::Types::Sequence sequence("", 1, 1);
	sequence.fixup();
	Poco::XSD::Types::OrderIterator itOrder = sequence.iterator();
	assertTrue(itOrder.end());
	assertTrue(itOrder.validNexts().empty());
	assertTrue(itOrder.canClose());
	itOrder.close();
	assertTrue(itOrder.end());
#endif

#ifdef ENABLE_XSD_PARSER
	Poco::XSD::Parser::CompactAttributes attributes;
	attributes["name"] = "item";
	assertEqual("item", attributes["name"]);
	assertEqual("http://www.w3.org/2001/XMLSchema", Poco::XSD::Parser::Constants::XSD_NAMESPACE_URI);
#endif

#ifdef ENABLE_XSD_VALIDATOR
	const std::string xsd(
		"<xs:schema xmlns:xs=\"http://www.w3.org/2001/XMLSchema\">"
		"<xs:element name=\"item\">"
		"<xs:complexType>"
		"<xs:attribute name=\"name\" type=\"xs:string\" use=\"required\"/>"
		"</xs:complexType>"
		"</xs:element>"
		"</xs:schema>");
	Poco::XSD::Validator::Validator::validate("<item name=\"A\"/>", xsd);
	try
	{
		Poco::XSD::Validator::Validator::validate("<item/>", xsd);
		failmsg("missing required attribute must fail schema validation");
	}
	catch (const Poco::DataFormatException&)
	{
	}
#endif
}


void ModulesTest::testGeo()
{
#ifdef ENABLE_GEO
	const Poco::Geo::LatLon sanFrancisco(Poco::Geo::LatLon::fromDegrees(37.728965, -122.420151));
	const Poco::Geo::LatLon newYork(Poco::Geo::LatLon::fromDegrees(40.712180, -73.995796));
	const double meters = newYork.greatCircleDistanceTo(sanFrancisco).radians()*Poco::Geo::LatLon::EARTH_MEAN_RADIUS;
	assertEqualDelta(4131767.3, meters, 0.1);

	Poco::Geo::LatLon first(sanFrancisco);
	Poco::Geo::LatLon second(newYork);
	swap(first, second);
	assertTrue(first == newYork);
	assertTrue(second == sanFrancisco);
#endif
}


void ModulesTest::testSerial()
{
#ifdef ENABLE_SERIAL
	static_assert(!std::is_copy_constructible_v<Poco::Serial::SerialPort>);
	static_assert(!std::is_copy_assignable_v<Poco::Serial::SerialPort>);
	static_assert(std::is_nothrow_move_constructible_v<Poco::Serial::SerialPort>);
	static_assert(std::is_nothrow_move_assignable_v<Poco::Serial::SerialPort>);

	const Poco::Serial::SerialPort port;
	assertFalse(port.isOpen());
	assertTrue(port.flowControl() == Poco::Serial::SerialPort::FLOW_NONE);
#endif
}


CppUnit::Test* ModulesTest::suite()
{
	CppUnit::TestSuite* pSuite = new CppUnit::TestSuite("ModulesTest");

	CppUnit_addTest(pSuite, ModulesTest, testFoundation);
#ifdef ENABLE_ENCODINGS
	CppUnit_addTest(pSuite, ModulesTest, testEncodings);
#endif
#ifdef ENABLE_XML
	CppUnit_addTest(pSuite, ModulesTest, testXML);
#endif
#ifdef ENABLE_JSON
	CppUnit_addTest(pSuite, ModulesTest, testJSON);
#endif
#ifdef ENABLE_UTIL
	CppUnit_addTest(pSuite, ModulesTest, testUtil);
#endif
#ifdef ENABLE_NET
	CppUnit_addTest(pSuite, ModulesTest, testNet);
#endif
#if defined(ENABLE_NETSSL_OPENSSL) || defined(ENABLE_NETSSL_WIN)
	CppUnit_addTest(pSuite, ModulesTest, testNetSSL);
#endif
#ifdef ENABLE_CRYPTO
	CppUnit_addTest(pSuite, ModulesTest, testCrypto);
#endif
#ifdef ENABLE_JWT
	CppUnit_addTest(pSuite, ModulesTest, testJWT);
#endif
#ifdef ENABLE_DATA
	CppUnit_addTest(pSuite, ModulesTest, testData);
#endif
#ifdef ENABLE_DATA_SQLITE
	CppUnit_addTest(pSuite, ModulesTest, testSQLite);
#endif
#ifdef ENABLE_DATA_ODBC
	CppUnit_addTest(pSuite, ModulesTest, testODBC);
#endif
#ifdef ENABLE_MONGODB
	CppUnit_addTest(pSuite, ModulesTest, testMongoDB);
#endif
#ifdef ENABLE_REDIS
	CppUnit_addTest(pSuite, ModulesTest, testRedis);
#endif
#ifdef ENABLE_PROMETHEUS
	CppUnit_addTest(pSuite, ModulesTest, testPrometheus);
#endif
#ifdef ENABLE_ZIP
	CppUnit_addTest(pSuite, ModulesTest, testZip);
#endif
#ifdef ENABLE_PDF
	CppUnit_addTest(pSuite, ModulesTest, testPDF);
#endif
#ifdef ENABLE_DNSSD
	CppUnit_addTest(pSuite, ModulesTest, testDNSSD);
#endif
#ifdef ENABLE_CPPPARSER
	CppUnit_addTest(pSuite, ModulesTest, testCppParser);
#endif
#ifdef ENABLE_SEVENZIP
	CppUnit_addTest(pSuite, ModulesTest, testSevenZip);
#endif
#ifdef ENABLE_ACTIVERECORD
	CppUnit_addTest(pSuite, ModulesTest, testActiveRecord);
#endif
#ifdef ENABLE_SSH
	CppUnit_addTest(pSuite, ModulesTest, testSSH);
#endif
#ifdef ENABLE_CODEGENERATION
	CppUnit_addTest(pSuite, ModulesTest, testCodeGeneration);
#endif
#ifdef ENABLE_REMOTINGNG
	CppUnit_addTest(pSuite, ModulesTest, testRemotingNG);
#endif
#if defined(ENABLE_XSD_TYPES) || defined(ENABLE_XSD_PARSER) || defined(ENABLE_XSD_VALIDATOR)
	CppUnit_addTest(pSuite, ModulesTest, testXSD);
#endif
#ifdef ENABLE_GEO
	CppUnit_addTest(pSuite, ModulesTest, testGeo);
#endif
#ifdef ENABLE_SERIAL
	CppUnit_addTest(pSuite, ModulesTest, testSerial);
#endif

	return pSuite;
}
