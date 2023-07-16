//
// ConcurrentQueue.cpp
//
// Library: Foundation
// Package: Core
// Module:	CincurrentQueue
//
// Copyright Copyright (c) 2013-2020, Cameron Desrochers. All rights reserved.
// Extracted from https://github.com/cameron314/concurrentqueue lib and adapted for poco: Bychuk Alexander 2023
//
// SPDX-License-Identifier:	BSL-1.0
//
#include "Poco/ConcurrentQueue.h"


namespace Poco {
namespace Details {
ConcurrentQueueProducerTypelessBase::ConcurrentQueueProducerTypelessBase()
	: next(nullptr), inactive(false), token(nullptr)
{ }

size_t hash_thread_id(thread_id_t id)
{
	static_assert(sizeof(thread_id_t) <= 8, "Expected a platform where thread IDs are at most 64-bit values");
	return static_cast<size_t>(hash_32_or_64<sizeof(thread_id_converter<thread_id_t>::thread_id_hash_t)>::hash(
		thread_id_converter<thread_id_t>::prehash(id)));
}

// ThreadExitNotifier

void ThreadExitNotifier::subscribe(ThreadExitListener *listener)
{
	auto& tlsInst = instance();
	Poco::FastMutex::ScopedLock guard(mutex());
	listener->next = tlsInst.tail;
	listener->chain = &tlsInst;
	tlsInst.tail = listener;
}

void ThreadExitNotifier::unsubscribe(ThreadExitListener *listener)
{
	Poco::FastMutex::ScopedLock guard(mutex());
	if (!listener->chain) {
		return;  // race with ~ThreadExitNotifier
	}
	auto& tlsInst = *listener->chain;
	listener->chain = nullptr;
	ThreadExitListener** prev = &tlsInst.tail;
	for (auto ptr = tlsInst.tail; ptr != nullptr; ptr = ptr->next) {
		if (ptr == listener) {
			*prev = ptr->next;
			break;
		}
		prev = &ptr->next;
	}
}

ThreadExitNotifier::ThreadExitNotifier() : tail(nullptr) { }

ThreadExitNotifier::~ThreadExitNotifier()
{
	// This thread is about to exit, let everyone know!
	assert(this == &instance() && "If this assert fails, you likely have a buggy compiler! Change the preprocessor conditions such that MOODYCAMEL_CPP11_THREAD_LOCAL_SUPPORTED is no longer defined.");
	Poco::FastMutex::ScopedLock guard(mutex());
	for (auto ptr = tail; ptr != nullptr; ptr = ptr->next) {
		ptr->chain = nullptr;
		ptr->callback(ptr->userData);
	}
}

ThreadExitNotifier &ThreadExitNotifier::instance()
{
	static Poco::ThreadLocal<ThreadExitNotifier> notifier;
	return *notifier;
}

Poco::FastMutex &ThreadExitNotifier::mutex()
{
	// Must be static because the ThreadExitNotifier could be destroyed while unsubscribe is called
	static Poco::FastMutex mutex;
	return mutex;
}

}

// ProducerToken

ProducerToken::ProducerToken(ProducerToken &&other) POCO_NOEXCEPT
	: producer(other.producer)
{
	other.producer = nullptr;
	if (producer != nullptr) {
		producer->token = this;
	}
}

ProducerToken &ProducerToken::operator=(ProducerToken &&other) POCO_NOEXCEPT
{
	swap(other);
	return *this;
}

void ProducerToken::swap(ProducerToken &other) POCO_NOEXCEPT
{
	std::swap(producer, other.producer);
	if (producer != nullptr) {
		producer->token = this;
	}
	if (other.producer != nullptr) {
		other.producer->token = &other;
	}
}

ProducerToken::~ProducerToken()
{
	if (producer != nullptr) {
		producer->token = nullptr;
		producer->inactive.store(true, std::memory_order_release);
	}
}

// ConsumerToken

ConsumerToken::ConsumerToken(ConsumerToken &&other) POCO_NOEXCEPT
	: initialOffset(other.initialOffset), lastKnownGlobalOffset(other.lastKnownGlobalOffset), itemsConsumedFromCurrent(other.itemsConsumedFromCurrent), currentProducer(other.currentProducer), desiredProducer(other.desiredProducer)
{
}

ConsumerToken &ConsumerToken::operator=(ConsumerToken &&other) POCO_NOEXCEPT
{
	swap(other);
	return *this;
}

void ConsumerToken::swap(ConsumerToken &other) POCO_NOEXCEPT
{
	std::swap(initialOffset, other.initialOffset);
	std::swap(lastKnownGlobalOffset, other.lastKnownGlobalOffset);
	std::swap(itemsConsumedFromCurrent, other.itemsConsumedFromCurrent);
	std::swap(currentProducer, other.currentProducer);
	std::swap(desiredProducer, other.desiredProducer);
}

// ConcurrentQueue



}

