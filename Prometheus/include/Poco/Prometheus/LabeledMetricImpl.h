//
// LabeledMetricImpl.h
//
// Library: Prometheus
// Package: Core
// Module:  LabeledMetric
//
// Definition of the MetricImpl class template.
//
// Copyright (c) 2022, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// SPDX-License-Identifier:	BSL-1.0
//


#ifndef Prometheus_LabeledMetricImpl_INCLUDED
#define Prometheus_LabeledMetricImpl_INCLUDED


#include "Poco/Prometheus/LabeledMetric.h"
#include "Poco/Prometheus/Exporter.h"
#include "Poco/AutoPtr.h"
#include "Poco/String.h"
#include "Poco/Format.h"
#include "Poco/Mutex.h"
#include <functional>
#include <memory>
#include <map>


using namespace std::string_literals;


namespace Poco {
namespace Prometheus {


template <typename S>
class LabeledMetricImpl: public LabeledMetric
	/// A helper class for implementing LabeledMetric classes such as Counter, Gauge and Histogram.
	///
	/// This class takes care of managing label values and samples.
{
public:
	using Sample = S;
	using ProcessingFunction = std::function<void(const std::vector<std::string>&, const Sample&)>;

	LabeledMetricImpl(Type type, const std::string& name):
		LabeledMetric(type, name)
	{
	}

	LabeledMetricImpl(Type type, const std::string& name, Registry* pRegistry):
		LabeledMetric(type, name, pRegistry)
	{
	}

	Sample& labels(const std::vector<std::string>& labelValues)
		/// Returns the Sample associated with the given label values.
		///
		/// If the sample does not exist yet, it is created.
		///
		/// The returned reference can be cached by the caller.
	{
		if (labelValues.size() != labelNames().size())
		{
			if (labelNames().empty())
				throw Poco::InvalidArgumentException(Poco::format("Metric %s does not have labels"s, name()));
			else
				throw Poco::InvalidArgumentException(Poco::format("Metric %s requires label values for %s"s, name(), Poco::cat(", "s, labelNames().begin(), labelNames().end())));
		}

		Poco::FastMutex::ScopedLock lock(_mutex);

		const auto it = _samples.find(labelValues);
		if (it != _samples.end())
		{
			return *it->second;
		}
		else
		{
			std::unique_ptr<Sample> pUniqueSample = createSample();
			Sample* pSample = pUniqueSample.get();
			_samples[labelValues] = std::move(pUniqueSample);
			return *pSample;
		}
	}

	const Sample& labels(const std::vector<std::string>& labelValues) const
		/// Returns the Sample associated with the given label values.
		///
		/// If the sample does not exist, a Poco::NotFoundException is thrown.
		///
		/// The returned reference can be cached by the caller.
	{
		if (labelValues.size() != labelNames().size())
			throw Poco::InvalidArgumentException(Poco::format("Metric %s requires label values for %s"s, name(), Poco::cat(", "s, labelNames().begin(), labelNames().end())));

		Poco::FastMutex::ScopedLock lock(_mutex);

		const auto it = _samples.find(labelValues);
		if (it != _samples.end())
		{
			return *it->second;
		}
		else
		{
			throw Poco::NotFoundException("Label values"s, Poco::cat("|"s, labelValues.begin(), labelValues.end()));
		}
	}

	void remove(const std::vector<std::string>& labelValues)
		/// Removes the sample associated with the given label values.
	{
		if (labelValues.size() != labelNames().size())
			throw Poco::InvalidArgumentException(Poco::format("Metric %s requires label values for %s"s, name(), Poco::cat(", "s, labelNames().begin(), labelNames().end())));

		if (labelNames().empty())
			throw Poco::InvalidAccessException("Metric has no labels"s);

		Poco::FastMutex::ScopedLock lock(_mutex);

		_samples.erase(labelValues);
	}

	void clear()
		/// Removes all samples.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		_samples.clear();
	}

	std::size_t sampleCount() const
		/// Returns the number of samples.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		return _samples.size();
	}

	template <typename Fn>
	void forEach(ProcessingFunction func) const
		/// Calls the given function for each Sample.
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		for (const auto& p: _samples)
		{
			func(p.first, *p.second);
		}
	}

	// Collector
	void exportTo(Exporter& exporter) const override
	{
		Poco::FastMutex::ScopedLock lock(_mutex);

		exporter.writeHeader(*this);
		for (const auto& p: _samples)
		{
			this->writeSample(exporter, p.first, *p.second);
		}
	}

protected:
	virtual std::unique_ptr<Sample> createSample() const = 0;
		/// Creates a new Sample. Must be overridden by subclasses.

	virtual void writeSample(Exporter& exporter, const std::vector<std::string>& labelValues, const Sample& sample) const
		/// Writes a Sample to the Exporter.
		///
		/// Must be overridden by subclasses, unless the subclass
		/// overrides the exportTo() method.
		///
		/// The default implementation does nothing.
	{
	}

	~LabeledMetricImpl() = default;
		/// Destroys the LabeledMetricImpl.

private:
	std::map<std::vector<std::string>, std::unique_ptr<Sample>> _samples;
	mutable Poco::FastMutex _mutex;
};


} } // namespace Poco::Prometheus


#endif // Prometheus_LabeledMetricImpl_INCLUDED
