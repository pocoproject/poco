// 
// WhiteBalance.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_WhiteBalance_INCLUDED
#define ONVIF_WhiteBalance_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=WhiteBalance
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API WhiteBalance
{
public:
	WhiteBalance();

	WhiteBalance(
		const std::string& mode, 
		float crGain, 
		float cbGain);

	virtual ~WhiteBalance();

	float getCbGain() const;

	float getCrGain() const;

	const std::string& getMode() const;

	void setCbGain(float val);

	void setCrGain(float val);

	void setMode(const std::string& val);

	void setMode(std::string&& val);

private:
	//@ name=Mode
	//@ order=0
	std::string _mode;

	//@ name=CrGain
	//@ order=1
	float _crGain;

	//@ name=CbGain
	//@ order=2
	float _cbGain;

};


inline float WhiteBalance::getCbGain() const
{
	return _cbGain;
}


inline float WhiteBalance::getCrGain() const
{
	return _crGain;
}


inline const std::string& WhiteBalance::getMode() const
{
	return _mode;
}


inline void WhiteBalance::setCbGain(float val)
{
	_cbGain = val;
}


inline void WhiteBalance::setCrGain(float val)
{
	_crGain = val;
}


inline void WhiteBalance::setMode(const std::string& val)
{
	_mode = val;
}


inline void WhiteBalance::setMode(std::string&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_WhiteBalance_INCLUDED
