//
// IDeviceIOBinding.h
//
// Package: Generated
// Module:  IDeviceIOBinding
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef IDeviceIOBinding_INCLUDED
#define IDeviceIOBinding_INCLUDED


#include "ONVIF/DeviceIO/DeviceIOBinding.h"
#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"


namespace ONVIF {
namespace DeviceIO {


class IDeviceIOBinding: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IDeviceIOBinding>;

	IDeviceIOBinding();
		/// Creates a IDeviceIOBinding.

	virtual ~IDeviceIOBinding();
		/// Destroys the IDeviceIOBinding.

	virtual void getAudioOutputConfiguration(const std::string& audioOutputToken, ONVIF::AudioOutputConfiguration& audioOutputConfiguration) = 0;

	virtual void getAudioOutputConfigurationOptions(const std::string& audioOutputToken, ONVIF::AudioOutputConfigurationOptions& audioOutputOptions) = 0;

	virtual void getAudioOutputs(std::vector<std::string>& token) = 0;

	virtual void getAudioSourceConfiguration(const std::string& audioSourceToken, ONVIF::AudioSourceConfiguration& audioSourceConfiguration) = 0;

	virtual void getAudioSourceConfigurationOptions(const std::string& audioSourceToken, ONVIF::AudioSourceConfigurationOptions& audioSourceOptions) = 0;

	virtual void getAudioSources(std::vector<std::string>& token) = 0;

	virtual void getDigitalInputConfigurationOptions(const Poco::Optional<std::string>& token, ONVIF::DeviceIO::DigitalInputConfigurationOptions& digitalInputOptions) = 0;

	virtual void getDigitalInputs(std::vector<ONVIF::DigitalInput>& digitalInputs) = 0;

	virtual void getRelayOutputOptions(const Poco::Optional<std::string>& relayOutputToken, std::vector<ONVIF::DeviceIO::RelayOutputOptions>& relayOutputOptions) = 0;

	virtual void getRelayOutputs(std::vector<ONVIF::RelayOutput>& relayOutputs) = 0;

	virtual void getSerialPortConfiguration(const std::string& serialPortToken, ONVIF::DeviceIO::SerialPortConfiguration& serialPortConfiguration) = 0;

	virtual void getSerialPortConfigurationOptions(const std::string& serialPortToken, ONVIF::DeviceIO::SerialPortConfigurationOptions& serialPortOptions) = 0;

	virtual void getSerialPorts(std::vector<ONVIF::DeviceIO::SerialPort>& serialPort) = 0;

	virtual void getServiceCapabilities(ONVIF::DeviceIO::Capabilities& capabilities) = 0;

	virtual void getVideoOutputConfiguration(const std::string& videoOutputToken, ONVIF::VideoOutputConfiguration& videoOutputConfiguration) = 0;

	virtual void getVideoOutputConfigurationOptions(const std::string& videoOutputToken, ONVIF::VideoOutputConfigurationOptions& videoOutputConfigurationOptions) = 0;

	virtual void getVideoOutputs(std::vector<ONVIF::VideoOutput>& videoOutputs) = 0;

	virtual void getVideoSourceConfiguration(const std::string& videoSourceToken, ONVIF::VideoSourceConfiguration& videoSourceConfiguration) = 0;

	virtual void getVideoSourceConfigurationOptions(const std::string& videoSourceToken, ONVIF::VideoSourceConfigurationOptions& videoSourceConfigurationOptions) = 0;

	virtual void getVideoSources(std::vector<std::string>& token) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

	virtual void sendReceiveSerialCommand(const Poco::Optional<std::string>& token, const Poco::SharedPtr<ONVIF::DeviceIO::SerialData>& serialData, const Poco::Optional<std::string>& timeOut, Poco::Optional<int> dataLength, const Poco::Optional<std::string>& delimiter, Poco::SharedPtr<ONVIF::DeviceIO::SerialData>& serialData2) = 0;

	virtual void setAudioOutputConfiguration(const ONVIF::AudioOutputConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setAudioSourceConfiguration(const ONVIF::AudioSourceConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setDigitalInputConfigurations(const std::vector<ONVIF::DigitalInput>& digitalInputs) = 0;

	virtual void setRelayOutputSettings(const ONVIF::RelayOutput& relayOutput) = 0;

	virtual void setRelayOutputState(const std::string& relayOutputToken, const std::string& logicalState) = 0;

	virtual void setSerialPortConfiguration(const ONVIF::DeviceIO::SerialPortConfiguration& serialPortConfiguration, bool forcePersistance) = 0;

	virtual void setVideoOutputConfiguration(const ONVIF::VideoOutputConfiguration& configuration, bool forcePersistence) = 0;

	virtual void setVideoSourceConfiguration(const ONVIF::VideoSourceConfiguration& configuration, bool forcePersistence) = 0;

};


} // namespace DeviceIO
} // namespace ONVIF


#endif // IDeviceIOBinding_INCLUDED

