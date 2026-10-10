// 
// Mpeg4Configuration.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Mpeg4Configuration_INCLUDED
#define ONVIF_Mpeg4Configuration_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=Mpeg4Configuration
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Mpeg4Configuration
{
public:
	Mpeg4Configuration();

	Mpeg4Configuration(
		int govLength, 
		const std::string& mpeg4Profile);

	virtual ~Mpeg4Configuration();

	int getGovLength() const;

	const std::string& getMpeg4Profile() const;

	void setGovLength(int val);

	void setMpeg4Profile(const std::string& val);

	void setMpeg4Profile(std::string&& val);

private:
	//@ name=GovLength
	//@ order=0
	int _govLength;

	//@ name=Mpeg4Profile
	//@ order=1
	std::string _mpeg4Profile;

};


inline int Mpeg4Configuration::getGovLength() const
{
	return _govLength;
}


inline const std::string& Mpeg4Configuration::getMpeg4Profile() const
{
	return _mpeg4Profile;
}


inline void Mpeg4Configuration::setGovLength(int val)
{
	_govLength = val;
}


inline void Mpeg4Configuration::setMpeg4Profile(const std::string& val)
{
	_mpeg4Profile = val;
}


inline void Mpeg4Configuration::setMpeg4Profile(std::string&& val)
{
	_mpeg4Profile = std::move(val);
}


} // ONVIF


#endif // ONVIF_Mpeg4Configuration_INCLUDED
