// 
// EventFilter.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_EventFilter_INCLUDED
#define ONVIF_EventFilter_INCLUDED


#include "OASIS/WSN/B2/Filter.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {


//@ name=EventFilter
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API EventFilter: public OASIS::WSN::B2::Filter
{
public:
	EventFilter();

	virtual ~EventFilter();

};


} // ONVIF


#endif // ONVIF_EventFilter_INCLUDED
