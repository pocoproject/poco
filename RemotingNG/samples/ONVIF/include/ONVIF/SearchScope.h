// 
// SearchScope.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_SearchScope_INCLUDED
#define ONVIF_SearchScope_INCLUDED


#include "ONVIF/ONVIF.h"
#include "ONVIF/SourceReference.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class SearchScopeExtension;
} 


namespace ONVIF {


//@ name=SearchScope
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API SearchScope
{
public:
	SearchScope();

	SearchScope(
		const std::vector<SourceReference>& includedSources, 
		const std::vector<std::string>& includedRecordings, 
		const Poco::Optional<std::string>& recordingInformationFilter, 
		const Poco::SharedPtr<SearchScopeExtension>& extension);

	virtual ~SearchScope();

	const Poco::SharedPtr<SearchScopeExtension>& getExtension() const;

	const std::vector<std::string>& getIncludedRecordings() const;

	std::vector<std::string>& getIncludedRecordings();

	const std::vector<SourceReference>& getIncludedSources() const;

	std::vector<SourceReference>& getIncludedSources();

	const Poco::Optional<std::string>& getRecordingInformationFilter() const;

	void setExtension(const Poco::SharedPtr<SearchScopeExtension>& val);

	void setExtension(Poco::SharedPtr<SearchScopeExtension>&& val);

	void setIncludedRecordings(const std::vector<std::string>& val);

	void setIncludedRecordings(std::vector<std::string>&& val);

	void setIncludedSources(const std::vector<SourceReference>& val);

	void setIncludedSources(std::vector<SourceReference>&& val);

	void setRecordingInformationFilter(const Poco::Optional<std::string>& val);

	void setRecordingInformationFilter(Poco::Optional<std::string>&& val);

private:
	//@ mandatory=false
	//@ name=IncludedSources
	//@ order=0
	std::vector<SourceReference> _includedSources;

	//@ mandatory=false
	//@ name=IncludedRecordings
	//@ order=1
	std::vector<std::string> _includedRecordings;

	//@ mandatory=false
	//@ name=RecordingInformationFilter
	//@ order=2
	Poco::Optional<std::string> _recordingInformationFilter;

	//@ mandatory=false
	//@ name=Extension
	//@ order=3
	Poco::SharedPtr<SearchScopeExtension> _extension;

};


inline const Poco::SharedPtr<SearchScopeExtension>& SearchScope::getExtension() const
{
	return _extension;
}


inline const std::vector<std::string>& SearchScope::getIncludedRecordings() const
{
	return _includedRecordings;
}


inline std::vector<std::string>& SearchScope::getIncludedRecordings()
{
	return _includedRecordings;
}


inline const std::vector<SourceReference>& SearchScope::getIncludedSources() const
{
	return _includedSources;
}


inline std::vector<SourceReference>& SearchScope::getIncludedSources()
{
	return _includedSources;
}


inline const Poco::Optional<std::string>& SearchScope::getRecordingInformationFilter() const
{
	return _recordingInformationFilter;
}


inline void SearchScope::setExtension(const Poco::SharedPtr<SearchScopeExtension>& val)
{
	_extension = val;
}


inline void SearchScope::setExtension(Poco::SharedPtr<SearchScopeExtension>&& val)
{
	_extension = std::move(val);
}


inline void SearchScope::setIncludedRecordings(const std::vector<std::string>& val)
{
	_includedRecordings = val;
}


inline void SearchScope::setIncludedRecordings(std::vector<std::string>&& val)
{
	_includedRecordings = std::move(val);
}


inline void SearchScope::setIncludedSources(const std::vector<SourceReference>& val)
{
	_includedSources = val;
}


inline void SearchScope::setIncludedSources(std::vector<SourceReference>&& val)
{
	_includedSources = std::move(val);
}


inline void SearchScope::setRecordingInformationFilter(const Poco::Optional<std::string>& val)
{
	_recordingInformationFilter = val;
}


inline void SearchScope::setRecordingInformationFilter(Poco::Optional<std::string>&& val)
{
	_recordingInformationFilter = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/SearchScopeExtension.h"


#endif // ONVIF_SearchScope_INCLUDED
