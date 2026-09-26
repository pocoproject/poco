//
// Variable.h
//
// Copyright (c) 2008-2012, Applied Informatics Software Engineering GmbH.
// All rights reserved.
//
// This is unpublished proprietary source code of Applied Informatics.
// The contents of this file may not be disclosed to third parties, 
// copied or duplicated in any form, in whole or in part.
//


#ifndef CodeGen_Variable_H_INCLUDED
#define CodeGen_Variable_H_INCLUDED


#include "TypeInfo.h"
#include "PropertyHolder.h"
#include "Poco/Poco.h"
#include "Utility.h"


class Variable: public PropertyHolder
{
public:
	enum Modifiers
	{
		V_ISVALUE = 0,
		V_ISREF = 1,
		V_ISCONST = 2,
		V_ISCONSTREF = V_ISCONST | V_ISREF,
		V_ISPOINTER = 4,
		V_ISREFPOINTER = V_ISREF | V_ISPOINTER,
		V_ISCONSTPOINTER = V_ISCONST | V_ISPOINTER,
		V_ISCONSTREFPOINTER = V_ISCONSTREF | V_ISPOINTER,
		V_ISPOINTERPOINTER = 8,
		V_ISREFPOINTERPOINTER = V_ISREF | V_ISPOINTERPOINTER,
		V_ISCONSTPOINTERPOINTER = V_ISCONST | V_ISPOINTERPOINTER,
		V_ISCONSTREFPOINTERPOINTER = V_ISCONSTREF | V_ISPOINTERPOINTER,
		V_ISVECTOR = 16,
		V_ISREFVECTOR = V_ISREF | V_ISVECTOR,
		V_ISCONSTVECTOR = V_ISCONST | V_ISVECTOR,
		V_ISCONSTREFVECTOR = V_ISCONSTREF | V_ISVECTOR,
		V_ISVECTORINNERPOINTER = V_ISPOINTER | V_ISVECTOR,
		V_ISREFVECTORINNERPOINTER = V_ISPOINTER | V_ISREFVECTOR,
		V_ISCONSTVECTORINNERPOINTER = V_ISPOINTER | V_ISCONSTVECTOR,
		V_ISCONSTREFVECTORINNERPOINTER = V_ISPOINTER | V_ISCONSTREFVECTOR,
		V_ISRVREF = 32,
		V_ISRVREFPOINTER = V_ISRVREF | V_ISPOINTER,
		V_ISRVREFVECTOR = V_ISRVREF | V_ISVECTOR
	};

	Variable(const std::string& name, const TypeInfo& type, Utility::Access acc, int order, Modifiers t, bool optional, bool isNillable);

	virtual ~Variable();

	bool isOptional() const;
		/// Returns if the variable is optional or mandatory.
		
	bool isNillable() const;
		/// Returns true if the variable is nillable.

	void setOptional(bool val);
	
	void setNillable(bool vale);

	bool isConst() const;

	bool isRef() const;

	bool isRVRef() const;

	bool isPointer() const;

	bool isPointerPointer() const;

	bool isVector() const;

	void setModifiers(Variable::Modifiers mod);

	Variable::Modifiers getModifiers() const;

	const std::string& getName() const;

	void setName(const std::string& name);

	const TypeInfo& getType() const;

	void setType(const TypeInfo& type);

	int getOrder() const;

	void setOrder(int order);

	Utility::Access getAccess() const;

private:
	std::string _name;
	TypeInfo    _type;
	int         _order;
	Modifiers   _mod;
	bool        _optional;
	bool        _nillable;
	Utility::Access _access;
};


//
// inlines
//
inline bool Variable::isOptional() const
{
	return _optional;
}


inline void Variable::setOptional(bool val)
{
	_optional = val;
}


inline bool Variable::isNillable() const
{
	return _nillable;
}


inline void Variable::setNillable(bool val)
{
	_nillable = val;
}


inline bool Variable::isConst() const
{
	return ((_mod & V_ISCONST) != 0);
}


inline bool Variable::isRef() const
{
	return ((_mod & V_ISREF) != 0);
}


inline bool Variable::isRVRef() const
{
	return ((_mod & V_ISRVREF) != 0);
}


inline bool Variable::isPointer() const
{
	return ((_mod & V_ISPOINTER) != 0);
}


inline bool Variable::isPointerPointer() const
{
	return ((_mod & V_ISPOINTERPOINTER) != 0);
}


inline bool Variable::isVector() const
{
	return ((_mod & V_ISVECTOR) != 0);
}


inline void Variable::setModifiers(Variable::Modifiers mod)
{
	_mod = mod;
}


inline Variable::Modifiers Variable::getModifiers() const
{
	return _mod;
}


inline const std::string& Variable::getName() const
{
	return _name;
}


inline void Variable::setName(const std::string& name)
{
	poco_assert (!name.empty());
	_name = name;
}


inline const TypeInfo& Variable::getType() const
{
	return _type;
}


inline void Variable::setType(const TypeInfo& type)
{
	_type = type;
}


inline int Variable::getOrder() const
{
	return _order;
}


inline void Variable::setOrder(int order)
{
	_order = order;
}


inline Utility::Access Variable::getAccess() const
{
	return _access;
}


#endif // CodeGen_Variable_H_INCLUDED
