//
// AvahiResponderImpl.cpp
//
// $Id: //poco/1.7/DNSSD/Avahi/src/AvahiResponderImpl.cpp#1 $
//
// Library: DNSSD/Avahi
// Package: Implementation
// Module:  AvahiResponderImpl
//
// Copyright (c) 2006-2024, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#include "Poco/DNSSD/Avahi/AvahiResponderImpl.h"
#include "Poco/DNSSD/DNSSDException.h"
#include "Poco/StringTokenizer.h"
#include <avahi-common/alternative.h>
#include <avahi-common/malloc.h>
#include <avahi-common/error.h>


namespace Poco {
namespace DNSSD {
namespace Avahi {


extern "C" int avahiPollFunc(struct pollfd *ufds, unsigned int nfds, int timeout, void *userdata)
{
	Poco::Mutex* pMutex = reinterpret_cast<Poco::Mutex*>(userdata);
	pMutex->unlock();
	int ret = poll(ufds, nfds, timeout);
	pMutex->lock();
	return ret;
}


extern "C" void onClientStateChange(AvahiClient* avahiClient, AvahiClientState state, void* userdata)
{
	try
	{
		AvahiResponderImpl* pResponder = reinterpret_cast<AvahiResponderImpl*>(userdata);
		pResponder->onClientStateChange(state);
	}
	catch (...)
	{
	}
}


extern "C" void onGroupStateChange(AvahiEntryGroup* avahiGroup, AvahiEntryGroupState state, void* userdata)
{
	try
	{
		AvahiResponderImpl* pResponder = reinterpret_cast<AvahiResponderImpl*>(userdata);
		pResponder->onGroupStateChange(avahiGroup, state);
	}
	catch (...)
	{
	}
}


AvahiResponderImpl::AvahiResponderImpl(Poco::DNSSD::DNSSDResponder& owner):
	_owner(owner),
	_browser(*this),
	_avahiPoll(0),
	_avahiClient(0),
	_running(false),
	_nextRecordId(1)
{
	_avahiPoll = avahi_simple_poll_new();
	if (!_avahiPoll) throw DNSSDException("Cannot create Avahi simple poll object");
	avahi_simple_poll_set_func(_avahiPoll, Poco::DNSSD::Avahi::avahiPollFunc, &_mutex);
	int error;
	_avahiClient = avahi_client_new(avahi_simple_poll_get(_avahiPoll), AVAHI_CLIENT_NO_FAIL, Poco::DNSSD::Avahi::onClientStateChange, this, &error);
	if (!_avahiClient)
	{
		avahi_simple_poll_free(_avahiPoll);
		throw DNSSDException("Cannot create Avahi client", describeError(error), error);
	}
}


AvahiResponderImpl::~AvahiResponderImpl()
{
	try
	{
		stop();
		avahi_client_free(_avahiClient);
		avahi_simple_poll_free(_avahiPoll);
	}
	catch (...)
	{
		poco_unexpected();
	}
}


DNSSDBrowser& AvahiResponderImpl::browser()
{
	return _browser;
}


ServiceHandle AvahiResponderImpl::registerService(const Service& service, int options)
{
	ScopedLock lock(*this);

	AvahiEntryGroup* avahiGroup = avahi_entry_group_new(_avahiClient, Poco::DNSSD::Avahi::onGroupStateChange, this);
	if (!avahiGroup) throw DNSSDException("Cannot create Avahi Entry Group");
	try
	{
		ServiceHandle serviceHandle(avahiGroup);
		RecordVec records;
		setupEntryGroup(avahiGroup, service, records, options, false);
		_services[serviceHandle].options = options;
		return serviceHandle;
	}
	catch (...)
	{
		avahi_entry_group_free(avahiGroup);
		throw;
	}
}


void AvahiResponderImpl::unregisterService(ServiceHandle& serviceHandle)
{
	ScopedLock lock(*this);

	ServiceMap::iterator it = _services.find(serviceHandle);
	if (it != _services.end())
	{
		_services.erase(it);
	}
	AvahiEntryGroup* avahiGroup = serviceHandle.cast<AvahiEntryGroup*>();
	avahi_entry_group_free(avahiGroup);
}


RecordHandle AvahiResponderImpl::addRecord(ServiceHandle serviceHandle, const Record& record)
{
	ScopedLock lock(*this);

	ServiceMap::iterator it = _services.find(serviceHandle);
	if (it != _services.end())
	{
		RecordInfo recordInfo;
		recordInfo.record = record;
		recordInfo.id = _nextRecordId++;
		it->second.records.push_back(recordInfo);
		setupEntryGroup(serviceHandle.cast<AvahiEntryGroup*>(), it->second.service, it->second.records, it->second.options, false);
		return RecordHandle(reinterpret_cast<void*>(recordInfo.id));
	}
	else throw Poco::InvalidArgumentException("Unknown ServiceHandle");
}


void AvahiResponderImpl::updateRecord(ServiceHandle serviceHandle, RecordHandle recordHandle, const Record& record)
{
	ScopedLock lock(*this);

	ServiceMap::iterator it = _services.find(serviceHandle);
	if (it != _services.end())
	{
		bool found = false;
		for (RecordVec::iterator itRec = it->second.records.begin(); itRec != it->second.records.end(); ++itRec)
		{
			if (itRec->id == static_cast<int>(reinterpret_cast<Poco::IntPtr>(recordHandle.cast<void*>())))
			{
				itRec->record = record;
				found = true;
				break;
			}
		}
		if (!found) throw Poco::NotFoundException("Record not found", record.name());
		setupEntryGroup(serviceHandle.cast<AvahiEntryGroup*>(), it->second.service, it->second.records, it->second.options, false);
	}
	else throw Poco::InvalidArgumentException("Unknown ServiceHandle");
}


void AvahiResponderImpl::removeRecord(ServiceHandle serviceHandle, RecordHandle& recordHandle)
{
	ScopedLock lock(*this);

	ServiceMap::iterator it = _services.find(serviceHandle);
	if (it != _services.end())
	{
		bool found = false;
		for (RecordVec::iterator itRec = it->second.records.begin(); itRec != it->second.records.end(); ++itRec)
		{
			if (itRec->id == static_cast<int>(reinterpret_cast<Poco::IntPtr>(recordHandle.cast<void*>())))
			{
				it->second.records.erase(itRec);
				recordHandle.reset();
				found = true;
				break;
			}
		}
		if (!found) throw Poco::NotFoundException("Record not found");
		setupEntryGroup(serviceHandle.cast<AvahiEntryGroup*>(), it->second.service, it->second.records, it->second.options, false);
	}
	else throw Poco::InvalidArgumentException("Unknown ServiceHandle");
}


void AvahiResponderImpl::start()
{
	if (!_running)
	{
		if (!_pollThread.isRunning())
		{
			_pollThread.start(*this);
		}
		if (!_avahiClientReady.tryWait(START_TIMEOUT))
		{
			std::string state;
			switch (avahi_client_get_state(_avahiClient))
			{
			case AVAHI_CLIENT_S_REGISTERING:
				state = "registering";
				break;
			case AVAHI_CLIENT_S_RUNNING:
				state = "running";
				break;
			case AVAHI_CLIENT_S_COLLISION:
				state = "collision";
				break;
			case AVAHI_CLIENT_FAILURE:
				state = "failure";
				break;
			case AVAHI_CLIENT_CONNECTING:
				state = "connecting";
			};
			throw DNSSDException("Avahi client not ready; current state", state);
		}
		_running = true;
	}
}


void AvahiResponderImpl::stop()
{
	if (_running)
	{
		{
			ScopedLock lock(*this);
			avahi_simple_poll_quit(_avahiPoll);
		}
		_pollThread.join();
		_running = false;
	}
}


const char* AvahiResponderImpl::describeError(int code)
{
	return avahi_strerror(code);
}


void AvahiResponderImpl::lock()
{
	_mutex.lock();
}


void AvahiResponderImpl::unlock()
{
	_mutex.unlock();
}


void AvahiResponderImpl::run()
{
	ScopedLock lock(*this);
	avahi_simple_poll_loop(_avahiPoll);
}


void AvahiResponderImpl::setupEntryGroup(AvahiEntryGroup* avahiGroup, const Service& service, const RecordVec& records, int options, bool rename)
{
	avahi_entry_group_reset(avahiGroup);
	AvahiStringList* txtList = createTXTRecord(service.properties());
	try
	{
		int ifIndex = service.networkInterface() == 0 ? AVAHI_IF_UNSPEC : service.networkInterface();
		std::string type = service.type();
		std::string subtypes;
		std::string::size_type pos = type.find(',');
		if (pos != std::string::npos)
		{
			subtypes.assign(type, pos + 1, type.size() - pos);
			type.resize(pos);
		}
		std::string name(service.name());
		if (name.empty()) name = avahi_client_get_host_name(_avahiClient);
		const char* domain = service.domain().empty() ? 0 : service.domain().c_str();
		const char* host = service.host().empty() ? 0 : service.host().c_str();

		int error = rename ? AVAHI_ERR_COLLISION : avahi_entry_group_add_service_strlst(
			avahiGroup,
			ifIndex,
			AVAHI_PROTO_UNSPEC,
			(AvahiPublishFlags) 0,
			name.c_str(),
			type.c_str(),
			domain,
			host,
			service.port(),
			txtList
		);
		while (error == AVAHI_ERR_COLLISION)
		{
			if (options & DNSSDResponder::REG_NO_AUTORENAME) throw DNSSDException("Cannot register service: " + name, describeError(error), error);
			const char* newName = avahi_alternative_service_name(name.c_str());
			name = newName;
			avahi_free(const_cast<char*>(newName));
			error = avahi_entry_group_add_service_strlst(
				avahiGroup,
				ifIndex,
				AVAHI_PROTO_UNSPEC,
				(AvahiPublishFlags) 0,
				name.c_str(),
				type.c_str(),
				domain,
				host,
				service.port(),
				txtList
			);
		}
		if (error) throw DNSSDException("Cannot add service to Avahi Entry Group: " + name, describeError(error), error);

		if (!subtypes.empty())
		{
			Poco::StringTokenizer tok(subtypes, ",", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
			for (Poco::StringTokenizer::Iterator it = tok.begin(); it != tok.end(); ++it)
			{
				error = avahi_entry_group_add_service_subtype(
					avahiGroup,
					service.networkInterface(),
					AVAHI_PROTO_UNSPEC,
					(AvahiPublishFlags) 0,
					name.c_str(),
					type.c_str(),
					domain,
					it->c_str()
				);
				if (error) throw DNSSDException("Cannot add service subtype to Avahi Entry Group", describeError(error), error);
			}
		}

		for (RecordVec::const_iterator it = records.begin(); it != records.end(); ++it)
		{
			error = avahi_entry_group_add_record(
				avahiGroup,
				ifIndex,
				AVAHI_PROTO_UNSPEC,
				(AvahiPublishFlags) 0,
				it->record.name().c_str(),
				it->record.clazz(),
				it->record.type(),
				it->record.ttl(),
				it->record.data(),
				it->record.length()
			);
			if (error) throw DNSSDException("Cannot add record to Avahi Entry Group: " + it->record.name(), describeError(error), error);
		}

		error = avahi_entry_group_commit(avahiGroup);
		if (error) throw DNSSDException("Cannot commit Avahi Entry Group", describeError(error), error);

		ServiceHandle serviceHandle(avahiGroup);
		_services[serviceHandle].service = Service(service.networkInterface(), name, "", service.type(), service.domain(), service.host(), service.port(), service.properties());

		avahi_string_list_free(txtList);
	}
	catch (...)
	{
		avahi_entry_group_reset(avahiGroup);
		avahi_string_list_free(txtList);
		throw;
	}
}


AvahiStringList* AvahiResponderImpl::createTXTRecord(const Service::Properties& properties)
{
	AvahiStringList* avahiList = 0;
	Service::Properties::ConstIterator itVers = properties.find("txtvers");
	Service::Properties::ConstIterator itEnd = properties.end();
	std::string entry;
	if (itVers != itEnd)
	{
		std::string entry(itVers->first);
		entry += '=';
		entry += itVers->second;
		avahiList = avahi_string_list_new(entry.c_str(), NULL);
	}

	Service::Properties::ConstIterator it = properties.begin();
	for (; it != itEnd; ++it)
	{
		if (it != itVers)
		{
			if (avahiList)
			{
				avahiList = avahi_string_list_add_pair_arbitrary(avahiList, it->first.c_str(), reinterpret_cast<const uint8_t*>(it->second.empty() ? NULL : it->second.c_str()), it->second.size());
			}
			else
			{
				std::string entry(it->first);
				if (!it->second.empty())
				{
					entry += '=';
					entry += it->second;
				}
				avahiList = avahi_string_list_new(entry.c_str(), NULL);
			}
		}
	}

	return avahiList;
}


void AvahiResponderImpl::onClientStateChange(AvahiClientState state)
{
	ScopedLock lock(*this);

	if (state == AVAHI_CLIENT_S_RUNNING)
	{
		_avahiClientReady.set();
		reregisterServices();
	}
}


void AvahiResponderImpl::onGroupStateChange(AvahiEntryGroup* avahiGroup, AvahiEntryGroupState state)
{
	ScopedLock lock(*this);

	ServiceHandle serviceHandle(avahiGroup);
	ServiceMap::iterator it = _services.find(serviceHandle);
	if (it != _services.end())
	{
		if (state == AVAHI_ENTRY_GROUP_ESTABLISHED)
		{
			DNSSDResponder::ServiceEventArgs args(serviceHandle, it->second.service);
			_owner.serviceRegistered(this, args);
		}
		else if (state == AVAHI_ENTRY_GROUP_COLLISION)
		{
			if (it->second.options & DNSSDResponder::REG_NO_AUTORENAME)
			{
				int error = AVAHI_ERR_COLLISION;
				DNSSDResponder::ErrorEventArgs args(serviceHandle, it->second.service, Error(it->second.service.networkInterface(), error, describeError(error)));
				_owner.serviceRegistrationFailed(this, args);
			}
			else
			{
				setupEntryGroup(avahiGroup, it->second.service, it->second.records, it->second.options, true);
			}
		}
		else if (state == AVAHI_ENTRY_GROUP_FAILURE)
		{
			int error = avahi_client_errno(_avahiClient);
			DNSSDResponder::ErrorEventArgs args(serviceHandle, it->second.service, Error(it->second.service.networkInterface(), error, describeError(error)));
			_owner.serviceRegistrationFailed(this, args);
		}
	}
}


void AvahiResponderImpl::reregisterServices()
{
	for (ServiceMap::iterator it = _services.begin(); it != _services.end(); ++it)
	{
		setupEntryGroup(it->first.cast<AvahiEntryGroup*>(), it->second.service, it->second.records, it->second.options, false);
	}
}


} } } // namespace Poco::DNSSD::Avahi


namespace Poco {
namespace DNSSD {


namespace
{
	Poco::DNSSD::Avahi::AvahiResponderImplFactory implFactory;
}


void initializeDNSSD()
{
	Poco::DNSSD::DNSSDResponder::registerImplFactory(implFactory);
}


void uninitializeDNSSD()
{
	Poco::DNSSD::DNSSDResponder::unregisterImplFactory();
}


} } // namespace Poco::DNSSD

