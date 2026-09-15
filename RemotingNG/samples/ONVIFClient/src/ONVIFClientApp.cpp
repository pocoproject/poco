//
// ONVIFClientApp.cpp
//
// This sample implements a client for ONVIF network cameras.
//
// Copyright (c) 2024, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#include "ONVIF/Device/DeviceBindingClientHelper.h"
#include "ONVIF/Media/MediaBindingClientHelper.h"
#include "Poco/RemotingNG/SOAP/TransportFactory.h"
#include "Poco/RemotingNG/SOAP/Transport.h"
#include "Poco/RemotingNG/SOAP/SOAPFaultException.h"
#include "Poco/RemotingNG/Proxy.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/Net/HTTPCredentials.h"
#include "Poco/Util/Application.h"
#include "Poco/Util/Option.h"
#include "Poco/Util/OptionSet.h"
#include "Poco/Util/IntValidator.h"
#include "Poco/Util/HelpFormatter.h"
#include "Poco/NumberParser.h"
#include "Poco/StreamCopier.h"
#include "Poco/FileStream.h"
#include "Poco/NullStream.h"
#include "Poco/Format.h"
#include <iostream>
#include <iomanip>


using Poco::Util::Application;
using Poco::Util::Option;
using Poco::Util::OptionSet;
using Poco::Util::OptionCallback;
using Poco::Util::HelpFormatter;
using namespace std::string_literals;


template <class I>
Poco::RemotingNG::SOAP::Transport& transportFromInterface(Poco::AutoPtr<I> pInterface)
{
    Poco::RemotingNG::Proxy::Ptr pProxy = pInterface.template cast<Poco::RemotingNG::Proxy>();
    if (pProxy)
    {
        return static_cast<Poco::RemotingNG::SOAP::Transport&>(pProxy->remoting__transport());
    }
    else throw Poco::BadCastException();
}


class ONVIFClientApp: public Application
{
public:
	ONVIFClientApp()
	{
		setUnixOptions(true);
		Poco::RemotingNG::SOAP::TransportFactory::registerFactory();
	}

	~ONVIFClientApp()
	{
		Poco::RemotingNG::SOAP::TransportFactory::unregisterFactory();
	}

protected:
	int main(const std::vector<std::string>& args)
	{
		if (_helpRequested || args.empty())
		{
			displayHelp();
			return Application::EXIT_OK;
		}
		
		try
		{
			for (const auto& arg: args)
			{
				if (arg == "dateTime")
				{
					doDateTime();
				}
				else if (arg == "capabilities")
				{
					doCapabilities();
				}
				else if (arg == "profiles")
				{
					doProfiles();
				}
				else if (arg == "snapshot")
				{
					doSnapshot();
				}
			}
		}
		catch (Poco::RemotingNG::SOAP::SOAPFaultException& exc)
		{
			std::cerr 
				<< exc.displayText() << "\n"
				<< "  Code:   " << exc.faultCode() << "\n"
				<< "  Reason: " << exc.faultReason() << std::endl;
		}
		catch (Poco::Exception& exc)
		{
			std::cerr << exc.displayText() << std::endl;
		}

		return Application::EXIT_OK;
	}

	void doDateTime()
	{
		auto pDevice = getDeviceBinding();
		ONVIF::SystemDateTime dateTime;
		pDevice->getSystemDateAndTime(dateTime);
		std::cout 
			<< std::boolalpha
			<< "Date/Time Type:     " << dateTime.getDateTimeType() << "\n"
			<< "Daylight Savings:   " << dateTime.getDaylightSavings() << "\n";
		if (dateTime.getTimeZone())
		{
			std::cout << "Time Zone:          " << dateTime.getTimeZone()->getTZ() << "\n";
		}
		if (dateTime.getUTCDateTime())
		{
			const auto& dt = *dateTime.getUTCDateTime();
			std::cout << "UTC Date/Time:      " << Poco::format("%04d-%02d-%02d %02d:%02d:%02d\n"s, 
				dt.getDate().getYear(),
				dt.getDate().getMonth(),
				dt.getDate().getDay(),
				dt.getTime().getHour(),
				dt.getTime().getMinute(),
				dt.getTime().getSecond());
		}
		if (dateTime.getLocalDateTime())
		{
			const auto& dt = *dateTime.getLocalDateTime();
			std::cout << "Local Date/Time:    " << Poco::format("%04d-%02d-%02d %02d:%02d:%02d\n"s, 
				dt.getDate().getYear(),
				dt.getDate().getMonth(),
				dt.getDate().getDay(),
				dt.getTime().getHour(),
				dt.getTime().getMinute(),
				dt.getTime().getSecond());
		}
	}

	void doCapabilities()
	{
		auto pDevice = getDeviceBinding();
		ONVIF::Capabilities capabilities;
		pDevice->getCapabilities({"All"s}, capabilities);
		if (capabilities.getDevice())
		{
			const auto& device = *capabilities.getDevice();
			std::cout 
				<< "Device:\n"
				<< "  XAddr:                      " << device.getXAddr().toString() << std::endl;
			if (device.getSystem())
			{
				const auto& system = *device.getSystem();
				std::cout
					<< std::boolalpha
					<< "  System:\n"
					<< "    Supported Versions:       " << toString(system.getSupportedVersions()) << "\n"
					<< "    Remote Discovery:         " << system.getRemoteDiscovery() << "\n"
					<< "    Discovery Resolve:        " << system.getDiscoveryResolve() << "\n"
					<< "    Discovery Bye:            " << system.getDiscoveryBye() << "\n"
					<< "    Firmware Upgrade:         " << system.getFirmwareUpgrade() << "\n"
					<< "    System Logging:           " << system.getSystemLogging() << "\n"
					<< "    System Backup:            " << system.getSystemBackup() << std::endl;
			}
			
			if (device.getNetwork())
			{
				const auto& network = *device.getNetwork();
				std::cout 
					<< std::boolalpha
					<< "  Network:\n"
					<< "    IP Filter:                " << network.getIPFilter().value(false) << "\n"
					<< "    IP Version 6:             " << network.getIPVersion6().value(false) << "\n"
					<< "    DynDNS:                   " << network.getDynDNS().value(false) << "\n"
					<< "    Zero Configuration:       " << network.getZeroConfiguration().value(false) << std::endl;
			}

			if (device.getIO())
			{
				const auto& io = *device.getIO();
				std::cout 
					<< std::boolalpha
					<< "  I/O:\n"
					<< "    Input Connectors:         " << io.getInputConnectors().value(0) << "\n"
					<< "    Relay Outputs:            " << io.getRelayOutputs().value(0) << std::endl;
			}

			if (device.getSecurity())
			{
				const auto& security = *device.getSecurity();
				std::cout 
					<< std::boolalpha
					<< "  Security:\n"
					<< "    Access Policy Config:     " << security.getAccessPolicyConfig() << "\n"
					<< "    Kerberos Token:           " << security.getKerberosToken() << "\n"
					<< "    REL Token:                " << security.getRELToken() << "\n"
					<< "    SAML Token:               " << security.getSAMLToken() << "\n"
					<< "    X-509 Token:              " << security.getX_509Token() << "\n"
					<< "    Onboard Key Generation:   " << security.getOnboardKeyGeneration() << "\n"
					<< "    TLS 1.1:                  " << security.getTLS1_1() << "\n"
					<< "    TLS 1.2:                  " << security.getTLS1_2() << std::endl;
			}
		}

		if (capabilities.getAnalytics())
		{
			const auto& analytics = *capabilities.getAnalytics();
			std::cout 
				<< std::boolalpha
				<< "Analytics:\n"
				<< "  XAddr:                      " << analytics.getXAddr().toString() << "\n"
				<< "  Analytics Module Support:   " << analytics.getAnalyticsModuleSupport() << "\n"
				<< "  Rule Support:               " << analytics.getRuleSupport() << std::endl;
		}

		if (capabilities.getEvents())
		{
			const auto& events = *capabilities.getEvents();
			std::cout
				<< std::boolalpha
				<< "Events:\n"
				<< "  XAddr:                      " << events.getXAddr().toString() << "\n"
				<< "    WS Pull Point Support:    " << events.getWSPullPointSupport() << "\n"
				<< "    WS Pausable Subscription: " << events.getWSPausableSubscriptionManagerInterfaceSupport() << "\n"
				<< "    WS Subscription Policy:   " << events.getWSSubscriptionPolicySupport() << std::endl;
		}

		if (capabilities.getImaging())
		{
			const auto& imaging = *capabilities.getImaging();
			std::cout 
				<< "Imaging:\n"
				<< "  XAddr:                      " << imaging.getXAddr().toString() << std::endl;
		}

		if (capabilities.getMedia())
		{
			const auto& media = *capabilities.getMedia();
			std::cout
				<< "Media:\n"
				<< "  XAddr:                      " << media.getXAddr().toString() << "\n"
				<< "  Streaming:" << std::endl;
			const auto& sc = media.getStreamingCapabilities();
			std::cout
				<< std::boolalpha
				<< "    RTP Multicast:            " << sc.getRTPMulticast().value(false)  << "\n"
				<< "    RTP RTSP TCP:             " << sc.getRTP_RTSP_TCP().value(false)  << "\n"
				<< "    RTP TCP:                  " << sc.getRTP_TCP().value(false) << std::endl;
		}

		if (capabilities.getPTZ())
		{
			const auto& ptz = *capabilities.getPTZ();
			std::cout 
				<< "PTZ:\n"
				<< "  XAddr:                      " << ptz.getXAddr().toString() << std::endl;
		}
	}

	void doProfiles()
	{
		auto pDevice = getDeviceBinding();
		ONVIF::Capabilities capabilities;
		pDevice->getCapabilities({"Media"s}, capabilities);
		if (capabilities.getMedia())
		{
			const Poco::URI mediaURI = capabilities.getMedia()->getXAddr();
			auto pMedia = getMediaBinding(mediaURI);

			std::vector<ONVIF::Profile> profiles;
			pMedia->getProfiles(profiles);

			int i = 0;
			for (const auto& p: profiles)
			{
				std::cout
					<< std::boolalpha
					<< "Profile: " << i++ << "\n"
					<< "  Name:                       " << p.getName() << "\n"
					<< "  Token:                      " << p.getToken() << "\n"
					<< "  Fixed:                      " << p.getFixed().value(false) << std::endl;

				if (p.getMetadataConfiguration())
				{
					const auto& meta = *p.getMetadataConfiguration();
					std::cout
						<< std::boolalpha
						<< "  Metadata: \n"
						<< "    Name:                     " << meta.getName() << "\n"
						<< "    Token:                    " << meta.getToken() << "\n"
						<< "    Analytics:                " << meta.getAnalytics().value(false) << "\n"
						<< "    Compression Type:         " << meta.getCompressionType().value("-"s) << "\n"
						<< "    Geo Location:             " << meta.getGeoLocation().value(false) << std::endl;
				}

				if (p.getVideoEncoderConfiguration())
				{
					const auto& video = *p.getVideoEncoderConfiguration();
					const auto& res = video.getResolution();
					std::cout
						<< std::boolalpha
						<< "  Video Encoder:\n"
						<< "    Name:                     " << video.getName() << "\n"
						<< "    Token:                    " << video.getToken() << "\n"
						<< "    Encoding:                 " << video.getEncoding() << "\n"
						<< "    Guaranteed Frame Rate:    " << video.getGuaranteedFrameRate().value(false) << "\n"
						<< "    Quality:                  " << video.getQuality() << "\n"
						<< "    Resolution:               " << res.getWidth() << " x " << res.getHeight() << "\n"
						<< "    Use Count:                " << video.getUseCount() << std::endl;
				}
			}
		}
	}

	void doSnapshot()
	{
		auto pDevice = getDeviceBinding();
		ONVIF::Capabilities capabilities;
		pDevice->getCapabilities({"Media"s}, capabilities);
		if (capabilities.getMedia())
		{
			const Poco::URI mediaURI = capabilities.getMedia()->getXAddr();
			auto pMedia = getMediaBinding(mediaURI);

			if (_profile.empty())
			{
				std::vector<ONVIF::Profile> profiles;
				pMedia->getProfiles(profiles);
				if (!profiles.empty())
				{
					_profile = profiles[0].getToken();
				}

			}
	
			if (!_profile.empty())
			{
				ONVIF::MediaUri mediaUri;
				pMedia->getSnapshotUri(_profile, mediaUri);
				Poco::URI uri = mediaUri.getUri();
				std::string filename = _snapshotFileName;
				if (filename.empty())
				{
					filename = _profile;
				}
				std::cout << "Downloading snapshot from " << uri.toString() << std::endl;
				downloadImage(uri, _username, _password, filename);
				std::cout << "Snapshot saved to " << filename << std::endl;
			}
		}
	}

	void downloadImage(const Poco::URI& uri, const std::string& username, const std::string& password, std::string& filename)
	{
		Poco::Net::HTTPClientSession cs(uri.getHost(), uri.getPort());
		Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_GET, uri.getPathEtc(), Poco::Net::HTTPMessage::HTTP_1_1);
		Poco::Net::HTTPResponse res;
		Poco::Net::HTTPCredentials creds(username, password);
		cs.sendRequest(req);
		std::istream* pResStream = &cs.receiveResponse(res);
		if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_UNAUTHORIZED)
		{
			Poco::NullOutputStream nullStream;
			Poco::StreamCopier::copyStream(*pResStream, nullStream);
			creds.authenticate(req, res);
			cs.sendRequest(req);
			pResStream = &cs.receiveResponse(res);
		}
		if (res.getStatus() == Poco::Net::HTTPResponse::HTTP_OK)
		{
			Poco::Path p(filename);
			if (p.getExtension().empty())
			{
				const std::string contentType = res.getContentType();
				if (contentType == "image/jpeg")
					p.setExtension("jpeg"s);
				else if (contentType == "image/png")
					p.setExtension("png"s);
				filename = p.toString();
			}
			Poco::FileOutputStream ostr(filename);
			Poco::StreamCopier::copyStream(*pResStream, ostr);
		}
	}

	static std::string toString(const std::vector<ONVIF::OnvifVersion>& versions)
	{
		std::string result;
		for (const auto& v: versions)
		{
			if (!result.empty()) result += ", ";
			result += Poco::format("%d.%d"s, v.getMajor(), v.getMinor());
		}
		return result;
	}

	ONVIF::Device::IDeviceBinding::Ptr getDeviceBinding() const
	{
		return ONVIF::Device::DeviceBindingClientHelper::find(Poco::format("http://%s:%hu/onvif/device_service"s, _host, _port), "soap-1.2"s);
	}

	ONVIF::Media::IMediaBinding::Ptr getMediaBinding(const Poco::URI& uri)
	{
		auto pMedia = ONVIF::Media::MediaBindingClientHelper::find(uri.toString(), "soap-1.2"s);
		auto& mediaTransport = transportFromInterface(pMedia);
		mediaTransport.setAuthentication(Poco::RemotingNG::SOAP::Transport::AUTH_WSSE_DIGEST);
		mediaTransport.setUsername(_username);
		mediaTransport.setPassword(_password);
		return pMedia;
	}

	void defineOptions(OptionSet& options)
	{
		Application::defineOptions(options);

		options.addOption(
			Option("help"s, "h"s, "Display help information on command line arguments."s)
				.required(false)
				.repeatable(false)
				.callback(OptionCallback<ONVIFClientApp>(this, &ONVIFClientApp::handleHelp)));

		options.addOption(
			Option("host"s, "H"s, "Specify camera IP address or domain name."s)
				.required(true)
				.repeatable(false)
				.argument("address"s)
				.callback(OptionCallback<ONVIFClientApp>(this, &ONVIFClientApp::handleHost)));

		options.addOption(
			Option("port"s, "P"s, "Specify camera ONVIF port number (default 80)."s)
				.required(false)
				.repeatable(false)
				.argument("port"s)
				.validator(new Poco::Util::IntValidator(1, 65535))
				.callback(OptionCallback<ONVIFClientApp>(this, &ONVIFClientApp::handlePort)));

		options.addOption(
			Option("username"s, "u"s, "Specify camera username for authentication."s)
				.required(true)
				.repeatable(false)
				.argument("username"s)
				.callback(OptionCallback<ONVIFClientApp>(this, &ONVIFClientApp::handleUsername)));

		options.addOption(
			Option("password"s, "p"s, "Specify camera password for authentication."s)
				.required(true)
				.repeatable(false)
				.argument("password"s)
				.callback(OptionCallback<ONVIFClientApp>(this, &ONVIFClientApp::handlePassword)));

		options.addOption(
			Option("profile"s, ""s, "Specify media profile name for snapshot image download."s)
				.required(false)
				.repeatable(false)
				.argument("name"s)
				.callback(OptionCallback<ONVIFClientApp>(this, &ONVIFClientApp::handleProfile)));

		options.addOption(
			Option("output"s, "o"s, "Specify snapshot image file name."s)
				.required(false)
				.repeatable(false)
				.argument("name"s)
				.callback(OptionCallback<ONVIFClientApp>(this, &ONVIFClientApp::handleOutput)));
	}

	void displayHelp()
	{
		HelpFormatter helpFormatter(options());
		helpFormatter.setCommand(commandName());
		helpFormatter.setUnixStyle(true);
		helpFormatter.setIndent(4);
		helpFormatter.setUsage("[dateTime|capabilities|profiles|snapshot] [OPTIONS]");
		helpFormatter.setHeader(
			"A simple client for ONVIF-capable network cameras.\n\n"
			"The following options are supported:");
		helpFormatter.setFooter(Poco::format(
			"Examples:\n"
			"  %[0]s --host 192.168.1.234 dateTime\n"
			"    -> Show camera system date and time.\n"
			"  %[0]s --host 192.168.1.234 --username onvif --password s3cr3t snapshot\n"
			"    -> Download a snapshot image.\n"s,
			commandName()));
		helpFormatter.format(std::cout);
	}
	
	void handleHelp(const std::string& name, const std::string& value)
	{
		_helpRequested = true;
		stopOptionsProcessing();
	}

	void handleHost(const std::string& name, const std::string& value)
	{
		_host = value;
	}

	void handlePort(const std::string& name, const std::string& value)
	{
		_port = static_cast<Poco::UInt16>(Poco::NumberParser::parse(value));
	}
	
	void handleUsername(const std::string& name, const std::string& value)
	{
		_username = value;
	}

	void handlePassword(const std::string& name, const std::string& value)
	{
		_password = value;
	}

	void handleProfile(const std::string& name, const std::string& value)
	{
		_profile = value;
	}

	void handleOutput(const std::string& name, const std::string& value)
	{
		_snapshotFileName = value;
	}

private:
	std::string _host;
	Poco::UInt16 _port = 80;
	std::string _username;
	std::string _password;
	std::string _profile;
	std::string _snapshotFileName;
	bool _helpRequested = false;
};


POCO_APP_MAIN(ONVIFClientApp)
