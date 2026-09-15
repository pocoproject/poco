//
// IConvertTemperature.h
//
// Package: Generated
// Module:  IConvertTemperature
//
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
//


#ifndef IConvertTemperature_INCLUDED
#define IConvertTemperature_INCLUDED


#include "Poco/AutoPtr.h"
#include "Poco/RefCountedObject.h"
#include "Poco/RemotingNG/Identifiable.h"
#include "WebServiceX/ConvertTemperature.h"


namespace WebServiceX {


class IConvertTemperature: public virtual Poco::RefCountedObject
{
public:
	using Ptr = Poco::AutoPtr<IConvertTemperature>;

	IConvertTemperature();
		/// Creates a IConvertTemperature.

	virtual ~IConvertTemperature();
		/// Destroys the IConvertTemperature.

	virtual void convertTemp(double temperature, const std::string& fromUnit, const std::string& toUnit, double& convertTempResult) = 0;

	static const Poco::RemotingNG::Identifiable::TypeId& remoting__typeId();
		/// Returns the TypeId of the class.

};


} // namespace WebServiceX


#endif // IConvertTemperature_INCLUDED

