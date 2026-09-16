//
// DeviceIOBindingProxy.h
//
// Package: Generated
// Module:  DeviceIOBindingProxy
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef DeviceIOBindingProxy_INCLUDED
#define DeviceIOBindingProxy_INCLUDED


#include "ONVIF/DeviceIO/IDeviceIOBinding.h"
#include "Poco/RemotingNG/Proxy.h"


namespace ONVIF {
namespace DeviceIO {


class DeviceIOBindingProxy: public ONVIF::DeviceIO::IDeviceIOBinding, public Poco::RemotingNG::Proxy
{
public:
	using Ptr = Poco::AutoPtr<DeviceIOBindingProxy>;

	DeviceIOBindingProxy(const Poco::RemotingNG::Identifiable::ObjectId& oid);
		/// Creates a DeviceIOBindingProxy.

	virtual ~DeviceIOBindingProxy();
		/// Destroys the DeviceIOBindingProxy.

	virtual void getAudioOutputConfiguration(const std::string& audioOutputToken, ONVIF::AudioOutputConfiguration& audioOutputConfiguration);

	virtual void getAudioOutputConfigurationOptions(const std::string& audioOutputToken, ONVIF::AudioOutputConfigurationOptions& audioOutputOptions);

	virtual void getAudioOutputs(std::vector<std::string>& token);

	virtual void getAudioSourceConfiguration(const std::string& audioSourceToken, ONVIF::AudioSourceConfiguration& audioSourceConfiguration);

	virtual void getAudioSourceConfigurationOptions(const std::string& audioSourceToken, ONVIF::AudioSourceConfigurationOptions& audioSourceOptions);

	virtual void getAudioSources(std::vector<std::string>& token);

	virtual void getDigitalInputConfigurationOptions(const Poco::Optional<std::string>& token, ONVIF::DeviceIO::DigitalInputConfigurationOptions& digitalInputOptions);

	virtual void getDigitalInputs(std::vector<ONVIF::DigitalInput>& digitalInputs);

	virtual void getRelayOutputOptions(const Poco::Optional<std::string>& relayOutputToken, std::vector<ONVIF::DeviceIO::RelayOutputOptions>& relayOutputOptions);

	virtual void getRelayOutputs(std::vector<ONVIF::RelayOutput>& relayOutputs);

	virtual void getSerialPortConfiguration(const std::string& serialPortToken, ONVIF::DeviceIO::SerialPortConfiguration& serialPortConfiguration);

	virtual void getSerialPortConfigurationOptions(const std::string& serialPortToken, ONVIF::DeviceIO::SerialPortConfigurationOptions& serialPortOptions);

	virtual void getSerialPorts(std::vector<ONVIF::DeviceIO::SerialPort>& serialPort);

	virtual void getServiceCapabilities(ONVIF::DeviceIO::Capabilities& capabilities);

	virtual void getVideoOutputConfiguration(const std::string& videoOutputToken, ONVIF::VideoOutputConfiguration& videoOutputConfiguration);

	virtual void getVideoOutputConfigurationOptions(const std::string& videoOutputToken, ONVIF::VideoOutputConfigurationOptions& videoOutputConfigurationOptions);

	virtual void getVideoOutputs(std::vector<ONVIF::VideoOutput>& videoOutputs);

	virtual void getVideoSourceConfiguration(const std::string& videoSourceToken, ONVIF::VideoSourceConfiguration& videoSourceConfiguration);

	virtual void getVideoSourceConfigurationOptions(const std::string& videoSourceToken, ONVIF::VideoSourceConfigurationOptions& videoSourceConfigurationOptions);

	virtual void getVideoSources(std::vector<std::string>& token);

	virtual const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId() const;

	virtual void sendReceiveSerialCommand(const Poco::Optional<std::string>& token, const Poco::SharedPtr<ONVIF::DeviceIO::SerialData>& serialData, const Poco::Optional<std::string>& timeOut, Poco::Optional<int> dataLength, const Poco::Optional<std::string>& delimiter, Poco::SharedPtr<ONVIF::DeviceIO::SerialData>& serialData2);

	virtual void setAudioOutputConfiguration(const ONVIF::AudioOutputConfiguration& configuration, bool forcePersistence);

	virtual void setAudioSourceConfiguration(const ONVIF::AudioSourceConfiguration& configuration, bool forcePersistence);

	virtual void setDigitalInputConfigurations(const std::vector<ONVIF::DigitalInput>& digitalInputs);

	virtual void setRelayOutputSettings(const ONVIF::RelayOutput& relayOutput);

	virtual void setRelayOutputState(const std::string& relayOutputToken, const std::string& logicalState);

	virtual void setSerialPortConfiguration(const ONVIF::DeviceIO::SerialPortConfiguration& serialPortConfiguration, bool forcePersistance);

	virtual void setVideoOutputConfiguration(const ONVIF::VideoOutputConfiguration& configuration, bool forcePersistence);

	virtual void setVideoSourceConfiguration(const ONVIF::VideoSourceConfiguration& configuration, bool forcePersistence);

private:
	static const std::string DEFAULT_NS;
};


inline const Poco::RemotingNG::Identifiable::TypeId& DeviceIOBindingProxy::remoting__typeId() const
{
	return IDeviceIOBinding::remoting__typeId();
}


} // namespace DeviceIO
} // namespace ONVIF


#endif // DeviceIOBindingProxy_INCLUDED

