//
// X509Revoked.cpp
//
//
// Library: Crypto
// Package: Certificate
// Module:  X509Revoked
//
// Copyright (c) 2006-2009, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/Crypto/X509Revoked.h"
#include "Poco/DateTimeParser.h"
#include <openssl/x509v3.h>

namespace Poco {
namespace Crypto {


X509Revoked::X509Revoked():
	_pRevoked(X509_REVOKED_new())
{

}


X509Revoked::X509Revoked(X509_REVOKED *pRevoked):
	_pRevoked(pRevoked)
{
	poco_check_ptr(_pRevoked);
}

X509Revoked::X509Revoked(const X509Revoked &revoked):
	_pRevoked(nullptr)
{
	duplicate(revoked._pRevoked, &_pRevoked);
}

X509Revoked::X509Revoked(X509Revoked&& other):
	_pRevoked(nullptr)
{
	swap(*this, other);
}


X509Revoked& X509Revoked::operator =(X509Revoked other)
{
	swap(*this, other);
	return *this;
}

void X509Revoked::swap(X509Revoked &first, X509Revoked& second)
{
	using std::swap;
	swap(first._pRevoked, second._pRevoked);
}


X509Revoked::~X509Revoked()
{
	X509_REVOKED_free(_pRevoked);
}


void X509Revoked::setSerialNumber(long serial)
{
	ASN1_INTEGER* asn1Serial = ASN1_INTEGER_new();
	ASN1_INTEGER_set(asn1Serial, serial);
	X509_REVOKED_set_serialNumber(_pRevoked, asn1Serial);
	ASN1_INTEGER_free(asn1Serial);
}


void X509Revoked::setSerialNumber(const std::string &serial)
{
	BIGNUM *bn = BN_new();
	int rc = BN_hex2bn(&bn, serial.data());
	if (rc)
	{
		ASN1_INTEGER* asn1Serial = ASN1_INTEGER_new();
		BN_to_ASN1_INTEGER(bn, asn1Serial);
		X509_REVOKED_set_serialNumber(_pRevoked, asn1Serial);
		ASN1_INTEGER_free(asn1Serial);
	}
	BN_free(bn);
}


std::string X509Revoked::serialNumber() const
{
	std::string result;
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	const ASN1_INTEGER* asn1serialNr = X509_REVOKED_get0_serialNumber(_pRevoked);
#else
	ASN1_INTEGER* asn1serialNr = _pRevoked->serialNumber;
#endif
	if (asn1serialNr)
	{
		BIGNUM* pBN = ASN1_INTEGER_to_BN(asn1serialNr, nullptr);
		if (pBN)
		{
			char* p = BN_bn2hex(pBN);
			if (p)
			{
				result = std::string(p);
				OPENSSL_free(p);
			}
			BN_free(pBN);
		}
	}
	return result;
}


void X509Revoked::setRevocationDate(DateTime date)
{
	Timestamp timestamp = date.timestamp();
	time_t time = timestamp.epochTime();
	ASN1_UTCTIME* asn1utctime = ASN1_UTCTIME_new();
	ASN1_UTCTIME_set(asn1utctime, time);
	X509_REVOKED_set_revocationDate(_pRevoked, asn1utctime);
	ASN1_UTCTIME_free(asn1utctime);
}


DateTime X509Revoked::getRevocationDate() const
{
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
	const ASN1_TIME* revokeTime = X509_REVOKED_get0_revocationDate(_pRevoked);
#else
	const ASN1_TIME* revokeTime = _pRevoked->revocationDate;
#endif
	std::string dateTime(reinterpret_cast<char*>(revokeTime->data));
	std::string fmt = "%y%m%d%H%M%S";
	if(revokeTime->type == V_ASN1_GENERALIZEDTIME)
	{
		fmt = "%Y%m%d%H%M%S";
	}
	int tzd;
	return DateTimeParser::parse(fmt, dateTime, tzd);
}


bool X509Revoked::setReasonCode(ReasonCode code)
{
	bool result = false;
	ASN1_ENUMERATED* asn1enum = ASN1_ENUMERATED_new();
	if (!asn1enum || !ASN1_ENUMERATED_set(asn1enum, code))
	{
		return result;
	}
	result = X509_REVOKED_add1_ext_i2d(_pRevoked, NID_crl_reason, asn1enum, 0, 0);
	if(asn1enum)
	{
		ASN1_ENUMERATED_free(asn1enum);
	}
	return result;
}

X509Revoked::ReasonCode X509Revoked::getReasonCode() const
{
	ReasonCode result = ReasonCode::UNSPECIFIED;
	int index = -1;
	ASN1_ENUMERATED* reason = static_cast<ASN1_ENUMERATED*>(X509_REVOKED_get_ext_d2i(_pRevoked, NID_crl_reason, &index, nullptr));
	if(!reason && (index == -1))
	{
		return result;
	}

	switch(ASN1_ENUMERATED_get(reason))
	{
	case 1:
		result = ReasonCode::KEY_COMPROMISE;
		break;
	case 2:
		result = ReasonCode::CA_COMPROMISE;
		break;
	case 3:
		result = ReasonCode::AFFILIATION_CHANGED;
		break;
	case 4:
		result = ReasonCode::SUPERSEDED;
		break;
	case 5:
		result = ReasonCode::CESSATION_OF_OPERATION;
		break;
	case 8:
		result = ReasonCode::REMOVE_FROM_CRL;
		break;
	default:
		result = ReasonCode::UNSPECIFIED;
	}

	ASN1_ENUMERATED_free(reason);
	return result;
}


void X509Revoked::addExtension(const X509Extension& x509Extension)
{
	X509_EXTENSION* ext = const_cast<X509_EXTENSION*>(static_cast<const X509_EXTENSION*>(x509Extension));
	X509_REVOKED_add_ext(_pRevoked, ext, -1);
}


X509Extension::List X509Revoked::findExtensionByNID(int nid)
{
	X509Extension::List extensions;
	int index = -1;
	do {
		index = X509_REVOKED_get_ext_by_NID(_pRevoked, nid, index);
		if (index == -1)
			break;

		X509_EXTENSION* ext = X509_REVOKED_get_ext(_pRevoked, index);
		ext = X509_EXTENSION_dup(ext);
		extensions.push_back(X509Extension(ext));
	}
	while(index >= 0);

	return extensions;
}


X509_REVOKED *X509Revoked::duplicate(const X509_REVOKED *pFromRevoked, X509_REVOKED **pToRevoked)
{
	X509_REVOKED* rev = const_cast<X509_REVOKED*>(pFromRevoked);
#if (OPENSSL_VERSION_NUMBER >=  0x1000200fL)
	*pToRevoked = X509_REVOKED_dup(rev);
#else
	*pToRevoked = X509_REVOKED_new();

	(*pToRevoked)->reason = rev->reason;
	(*pToRevoked)->sequence = rev->sequence;

	X509_REVOKED_set_serialNumber(*pToRevoked, rev->serialNumber);
	X509_REVOKED_set_revocationDate(*pToRevoked, rev->revocationDate);

	int size = sk_X509_EXTENSION_num(rev->extensions);
	for(int i = 0; i < size; i++ )
	{
		X509_EXTENSION* ex = sk_X509_EXTENSION_value(rev->extensions, i);
		X509_EXTENSION* newEx;
		X509Extension::duplicate(ex, &newEx);
		X509_REVOKED_add_ext(*pToRevoked, newEx, -1);
	}

	size = sk_GENERAL_NAME_num(rev->issuer);
	for(int i = 0; i < size; i++ )
	{
		GENERAL_NAME* na = sk_GENERAL_NAME_value(rev->issuer, i);
		GENERAL_NAME* newNa = GENERAL_NAME_dup(na);
		sk_GENERAL_NAME_push((*pToRevoked)->issuer, newNa);
	}
#endif
	return *pToRevoked;
}

} } // namespace Poco::Crypto
