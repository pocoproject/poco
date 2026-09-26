//
// Parameter.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef CodeGen_Parameter_H_INCLUDED
#define CodeGen_Parameter_H_INCLUDED


#include "Variable.h"


class Parameter: public Variable
{
public:
	enum Direction
	{
		DIR_IN,
		DIR_OUT,
		DIR_INOUT
	};
	
	Parameter(const std::string& name, 
		const TypeInfo& type, 
		int order,
		Variable::Modifiers t, 
		bool optional, 
		bool soapHeader,
		bool nillable);

	~Parameter();

	bool getSoapHeader() const;
		/// Returns if the parameter should be sent in the soapheader

	void setSoapHeader(bool soapHeader);
	
	void setDirection(Direction dir);
	
	Direction getDirection() const;

private:
	bool _soapHeader;
	Direction _direction;
};


//
// inlines
//
inline bool Parameter::getSoapHeader() const
{
	return _soapHeader;
}


inline void Parameter::setSoapHeader(bool soapHeader)
{
	_soapHeader = soapHeader;
}


inline void Parameter::setDirection(Direction dir)
{
	_direction = dir;
}


inline Parameter::Direction Parameter::getDirection() const
{
	return _direction;
}


#endif // CodeGen_Parameter_H_INCLUDED
