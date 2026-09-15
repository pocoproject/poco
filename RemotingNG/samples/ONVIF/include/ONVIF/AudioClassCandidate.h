// 
// AudioClassCandidate.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_AudioClassCandidate_INCLUDED
#define ONVIF_AudioClassCandidate_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=AudioClassCandidate
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API AudioClassCandidate
{
public:
	AudioClassCandidate();

	AudioClassCandidate(
		const std::string& type, 
		float likelihood);

	virtual ~AudioClassCandidate();

	float getLikelihood() const;

	const std::string& getType() const;

	void setLikelihood(float val);

	void setType(const std::string& val);

	void setType(std::string&& val);

private:
	//@ name=Type
	//@ order=0
	std::string _type;

	//@ name=Likelihood
	//@ order=1
	float _likelihood;

};


inline float AudioClassCandidate::getLikelihood() const
{
	return _likelihood;
}


inline const std::string& AudioClassCandidate::getType() const
{
	return _type;
}


inline void AudioClassCandidate::setLikelihood(float val)
{
	_likelihood = val;
}


inline void AudioClassCandidate::setType(const std::string& val)
{
	_type = val;
}


inline void AudioClassCandidate::setType(std::string&& val)
{
	_type = std::move(val);
}


} // ONVIF


#endif // ONVIF_AudioClassCandidate_INCLUDED
