// 
// Exception.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef Pizzeria_Exception_INCLUDED
#define Pizzeria_Exception_INCLUDED


#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>


namespace Pizzeria {


//@ name=Exception
//@ namespace="http://www.appinf.com/remoting"
//@ serialize
class Exception
{
public:
	Exception();

	Exception(
		const std::string& type, 
		const std::string& message, 
		int code);

	virtual ~Exception();

	int getCode() const;

	const std::string& getMessage() const;

	const std::string& getType() const;

	void setCode(int val);

	void setMessage(const std::string& val);

	void setType(const std::string& val);

private:
	//@ name=Type
	//@ order=0
	std::string _type;

	//@ name=Message
	//@ order=1
	std::string _message;

	//@ name=Code
	//@ order=2
	int _code;

};


inline int Exception::getCode() const
{
	return _code;
}


inline const std::string& Exception::getMessage() const
{
	return _message;
}


inline const std::string& Exception::getType() const
{
	return _type;
}


inline void Exception::setCode(int val)
{
	_code = val;
}


inline void Exception::setMessage(const std::string& val)
{
	_message = val;
}


inline void Exception::setType(const std::string& val)
{
	_type = val;
}


} // Pizzeria


#endif // Pizzeria_Exception_INCLUDED
