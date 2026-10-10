// 
// TopicSet.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef OASIS_WSN_T1_TopicSet_INCLUDED
#define OASIS_WSN_T1_TopicSet_INCLUDED


#include "OASIS/WSN/T1/ExtensibleDocumented.h"
#include "ONVIF/ONVIF.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace OASIS {
namespace WSN {
namespace T1 {


//@ name=TopicSetType
//@ namespace="http://docs.oasis-open.org/wsn/t-1"
//@ serialize
class ONVIF_API TopicSet: public ExtensibleDocumented
{
public:
	TopicSet();

	TopicSet(const Poco::SharedPtr<Documentation>& documentation);

	virtual ~TopicSet();

};


} } } // OASIS::WSN::T1


#endif // OASIS_WSN_T1_TopicSet_INCLUDED
