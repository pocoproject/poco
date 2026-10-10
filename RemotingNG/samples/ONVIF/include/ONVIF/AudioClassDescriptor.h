// 
// AudioClassDescriptor.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioClassDescriptor_INCLUDED
#define ONVIF_AudioClassDescriptor_INCLUDED


#include "ONVIF/AudioClassCandidate.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class AudioClassDescriptorExtension;
} 


namespace ONVIF {


//@ name=AudioClassDescriptor
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioClassDescriptor
{
public:
	AudioClassDescriptor();

	AudioClassDescriptor(
		const std::vector<AudioClassCandidate>& classCandidate, 
		const Poco::SharedPtr<AudioClassDescriptorExtension>& extension);

	virtual ~AudioClassDescriptor();

	const std::vector<AudioClassCandidate>& getClassCandidate() const;

	std::vector<AudioClassCandidate>& getClassCandidate();

	const Poco::SharedPtr<AudioClassDescriptorExtension>& getExtension() const;

	void setClassCandidate(const std::vector<AudioClassCandidate>& val);

	void setClassCandidate(std::vector<AudioClassCandidate>&& val);

	void setExtension(const Poco::SharedPtr<AudioClassDescriptorExtension>& val);

	void setExtension(Poco::SharedPtr<AudioClassDescriptorExtension>&& val);

private:
	//@ mandatory=false
	//@ name=ClassCandidate
	//@ order=0
	std::vector<AudioClassCandidate> _classCandidate;

	//@ mandatory=false
	//@ name=Extension
	//@ order=1
	Poco::SharedPtr<AudioClassDescriptorExtension> _extension;

};


inline const std::vector<AudioClassCandidate>& AudioClassDescriptor::getClassCandidate() const
{
	return _classCandidate;
}


inline std::vector<AudioClassCandidate>& AudioClassDescriptor::getClassCandidate()
{
	return _classCandidate;
}


inline const Poco::SharedPtr<AudioClassDescriptorExtension>& AudioClassDescriptor::getExtension() const
{
	return _extension;
}


inline void AudioClassDescriptor::setClassCandidate(const std::vector<AudioClassCandidate>& val)
{
	_classCandidate = val;
}


inline void AudioClassDescriptor::setClassCandidate(std::vector<AudioClassCandidate>&& val)
{
	_classCandidate = std::move(val);
}


inline void AudioClassDescriptor::setExtension(const Poco::SharedPtr<AudioClassDescriptorExtension>& val)
{
	_extension = val;
}


inline void AudioClassDescriptor::setExtension(Poco::SharedPtr<AudioClassDescriptorExtension>&& val)
{
	_extension = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/AudioClassDescriptorExtension.h"


#endif // ONVIF_AudioClassDescriptor_INCLUDED
