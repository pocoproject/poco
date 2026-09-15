// 
// PTControlDirectionOptions.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTControlDirectionOptions_INCLUDED
#define ONVIF_PTControlDirectionOptions_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class EFlipOptions;
class PTControlDirectionOptionsExtension;
class ReverseOptions;
} 


namespace ONVIF {


//@ name=PTControlDirectionOptions
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTControlDirectionOptions
{
public:
	PTControlDirectionOptions();

	PTControlDirectionOptions(
		const Poco::SharedPtr<EFlipOptions>& eFlip, 
		const Poco::SharedPtr<ReverseOptions>& reverse, 
		const Poco::SharedPtr<PTControlDirectionOptionsExtension>& extension);

	virtual ~PTControlDirectionOptions();

	const Poco::SharedPtr<EFlipOptions>& getEFlip() const;

	const Poco::SharedPtr<PTControlDirectionOptionsExtension>& getExtension() const;

	const Poco::SharedPtr<ReverseOptions>& getReverse() const;

	void setEFlip(const Poco::SharedPtr<EFlipOptions>& val);

	void setEFlip(Poco::SharedPtr<EFlipOptions>&& val);

	void setExtension(const Poco::SharedPtr<PTControlDirectionOptionsExtension>& val);

	void setExtension(Poco::SharedPtr<PTControlDirectionOptionsExtension>&& val);

	void setReverse(const Poco::SharedPtr<ReverseOptions>& val);

	void setReverse(Poco::SharedPtr<ReverseOptions>&& val);

private:
	//@ mandatory=false
	//@ name=EFlip
	//@ order=0
	Poco::SharedPtr<EFlipOptions> _eFlip;

	//@ mandatory=false
	//@ name=Reverse
	//@ order=1
	Poco::SharedPtr<ReverseOptions> _reverse;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<PTControlDirectionOptionsExtension> _extension;

};


inline const Poco::SharedPtr<EFlipOptions>& PTControlDirectionOptions::getEFlip() const
{
	return _eFlip;
}


inline const Poco::SharedPtr<PTControlDirectionOptionsExtension>& PTControlDirectionOptions::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<ReverseOptions>& PTControlDirectionOptions::getReverse() const
{
	return _reverse;
}


inline void PTControlDirectionOptions::setEFlip(const Poco::SharedPtr<EFlipOptions>& val)
{
	_eFlip = val;
}


inline void PTControlDirectionOptions::setEFlip(Poco::SharedPtr<EFlipOptions>&& val)
{
	_eFlip = std::move(val);
}


inline void PTControlDirectionOptions::setExtension(const Poco::SharedPtr<PTControlDirectionOptionsExtension>& val)
{
	_extension = val;
}


inline void PTControlDirectionOptions::setExtension(Poco::SharedPtr<PTControlDirectionOptionsExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTControlDirectionOptions::setReverse(const Poco::SharedPtr<ReverseOptions>& val)
{
	_reverse = val;
}


inline void PTControlDirectionOptions::setReverse(Poco::SharedPtr<ReverseOptions>&& val)
{
	_reverse = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/EFlipOptions.h"
#include "ONVIF/PTControlDirectionOptionsExtension.h"
#include "ONVIF/ReverseOptions.h"


#endif // ONVIF_PTControlDirectionOptions_INCLUDED
