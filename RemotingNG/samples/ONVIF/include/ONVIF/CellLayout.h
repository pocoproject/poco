// 
// CellLayout.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_CellLayout_INCLUDED
#define ONVIF_CellLayout_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/Transformation.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=CellLayout
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API CellLayout
{
public:
	CellLayout();

	CellLayout(
		int columns, 
		int rows, 
		const Transformation& transformation);

	virtual ~CellLayout();

	int getColumns() const;

	int getRows() const;

	const Transformation& getTransformation() const;

	void setColumns(int val);

	void setRows(int val);

	void setTransformation(const Transformation& val);

	void setTransformation(Transformation&& val);

private:
	//@ name=Columns
	//@ order=0
	//@ type=attr
	int _columns;

	//@ name=Rows
	//@ order=1
	//@ type=attr
	int _rows;

	//@ name=Transformation
	//@ order=2
	Transformation _transformation;

};


inline int CellLayout::getColumns() const
{
	return _columns;
}


inline int CellLayout::getRows() const
{
	return _rows;
}


inline const Transformation& CellLayout::getTransformation() const
{
	return _transformation;
}


inline void CellLayout::setColumns(int val)
{
	_columns = val;
}


inline void CellLayout::setRows(int val)
{
	_rows = val;
}


inline void CellLayout::setTransformation(const Transformation& val)
{
	_transformation = val;
}


inline void CellLayout::setTransformation(Transformation&& val)
{
	_transformation = std::move(val);
}


} // ONVIF


#endif // ONVIF_CellLayout_INCLUDED
