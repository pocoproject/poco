//
// MailRecipient.cpp
//
// Library: Net
// Package: Mail
// Module:  MailRecipient
//
// Copyright (c) 2005-2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Net/MailRecipient.h"
#include <algorithm>
#include <utility>


namespace Poco {
namespace Net {


MailRecipient::MailRecipient():
	_type(PRIMARY_RECIPIENT)
{
}


MailRecipient::MailRecipient(const MailRecipient& recipient)
	
= default;


MailRecipient::MailRecipient(RecipientType type, std::string  address):
	_address(std::move(address)),
	_type(type)
{
}


MailRecipient::MailRecipient(RecipientType type, std::string  address, std::string  realName):
	_address(std::move(address)),
	_realName(std::move(realName)),
	_type(type)
{
}


MailRecipient::~MailRecipient()
= default;


MailRecipient& MailRecipient::operator = (const MailRecipient& recipient)
{
	if (this != &recipient)
	{
		MailRecipient tmp(recipient);
		swap(tmp);
	}
	return *this;
}


void MailRecipient::swap(MailRecipient& recipient) noexcept
{
	std::swap(_type, recipient._type);
	std::swap(_address, recipient._address);
	std::swap(_realName, recipient._realName);
}


void MailRecipient::setType(RecipientType type)
{
	_type = type;
}


void MailRecipient::setAddress(const std::string& address)
{
	_address = address;
}


void MailRecipient::setRealName(const std::string& realName)
{
	_realName = realName;
}


} } // namespace Poco::Net
