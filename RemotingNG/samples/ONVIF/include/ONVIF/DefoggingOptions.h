// 
// DefoggingOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_DefoggingOptions_INCLUDED
#define ONVIF_DefoggingOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=DefoggingOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API DefoggingOptions
{
public:
	DefoggingOptions();

	DefoggingOptions(
		const std::vector<std::string>& mode, 
		bool level);

	virtual ~DefoggingOptions();

	bool getLevel() const;

	const std::vector<std::string>& getMode() const;

	std::vector<std::string>& getMode();

	void setLevel(bool val);

	void setMode(const std::vector<std::string>& val);

	void setMode(std::vector<std::string>&& val);

private:
	//@ name=Mode
	//@ order=0
	std::vector<std::string> _mode;

	//@ name=Level
	//@ order=1
	bool _level;

};


inline bool DefoggingOptions::getLevel() const
{
	return _level;
}


inline const std::vector<std::string>& DefoggingOptions::getMode() const
{
	return _mode;
}


inline std::vector<std::string>& DefoggingOptions::getMode()
{
	return _mode;
}


inline void DefoggingOptions::setLevel(bool val)
{
	_level = val;
}


inline void DefoggingOptions::setMode(const std::vector<std::string>& val)
{
	_mode = val;
}


inline void DefoggingOptions::setMode(std::vector<std::string>&& val)
{
	_mode = std::move(val);
}


} // ONVIF


#endif // ONVIF_DefoggingOptions_INCLUDED
