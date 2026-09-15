// 
// Transformation.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_Transformation_INCLUDED
#define ONVIF_Transformation_INCLUDED


#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class TransformationExtension;
class Vector;
} 


namespace ONVIF {


//@ name=Transformation
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API Transformation
{
public:
	Transformation();

	Transformation(
		const Poco::SharedPtr<Vector>& translate, 
		const Poco::SharedPtr<Vector>& scale, 
		const Poco::SharedPtr<TransformationExtension>& extension);

	virtual ~Transformation();

	const Poco::SharedPtr<TransformationExtension>& getExtension() const;

	const Poco::SharedPtr<Vector>& getScale() const;

	const Poco::SharedPtr<Vector>& getTranslate() const;

	void setExtension(const Poco::SharedPtr<TransformationExtension>& val);

	void setExtension(Poco::SharedPtr<TransformationExtension>&& val);

	void setScale(const Poco::SharedPtr<Vector>& val);

	void setScale(Poco::SharedPtr<Vector>&& val);

	void setTranslate(const Poco::SharedPtr<Vector>& val);

	void setTranslate(Poco::SharedPtr<Vector>&& val);

private:
	//@ mandatory=false
	//@ name=Translate
	//@ order=0
	Poco::SharedPtr<Vector> _translate;

	//@ mandatory=false
	//@ name=Scale
	//@ order=1
	Poco::SharedPtr<Vector> _scale;

	//@ mandatory=false
	//@ name=Extension
	//@ order=2
	Poco::SharedPtr<TransformationExtension> _extension;

};


inline const Poco::SharedPtr<TransformationExtension>& Transformation::getExtension() const
{
	return _extension;
}


inline const Poco::SharedPtr<Vector>& Transformation::getScale() const
{
	return _scale;
}


inline const Poco::SharedPtr<Vector>& Transformation::getTranslate() const
{
	return _translate;
}


inline void Transformation::setExtension(const Poco::SharedPtr<TransformationExtension>& val)
{
	_extension = val;
}


inline void Transformation::setExtension(Poco::SharedPtr<TransformationExtension>&& val)
{
	_extension = std::move(val);
}


inline void Transformation::setScale(const Poco::SharedPtr<Vector>& val)
{
	_scale = val;
}


inline void Transformation::setScale(Poco::SharedPtr<Vector>&& val)
{
	_scale = std::move(val);
}


inline void Transformation::setTranslate(const Poco::SharedPtr<Vector>& val)
{
	_translate = val;
}


inline void Transformation::setTranslate(Poco::SharedPtr<Vector>&& val)
{
	_translate = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/TransformationExtension.h"
#include "ONVIF/Vector.h"


#endif // ONVIF_Transformation_INCLUDED
