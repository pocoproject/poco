// 
// PTZNode.h
// 
// This file has been generated.
// Warning: All changes to this will be lost when the file is re-generated.
// 
// 


#ifndef ONVIF_PTZNode_INCLUDED
#define ONVIF_PTZNode_INCLUDED


#include "ONVIF/DeviceEntity.h"
#include "ONVIF/ONVIF.h"
#include "ONVIF/PTZSpaces.h"
#include "Poco/Nullable.h"
#include "Poco/Optional.h"
#include "Poco/SharedPtr.h"
#include <string>
#include <vector>


namespace ONVIF {
class PTZNodeExtension;
} 


namespace ONVIF {


//@ name=PTZNode
//@ namespace="http://www.onvif.org/ver10/schema"
//@ serialize
class ONVIF_API PTZNode: public DeviceEntity
{
public:
	PTZNode();

	PTZNode(
		const std::string& token, 
		Poco::Optional<bool> fixedHomePosition, 
		Poco::Optional<bool> geoMove, 
		const Poco::Optional<std::string>& name, 
		const PTZSpaces& supportedPTZSpaces, 
		int maximumNumberOfPresets, 
		bool homeSupported, 
		const std::vector<std::string>& auxiliaryCommands, 
		const Poco::SharedPtr<PTZNodeExtension>& extension);

	virtual ~PTZNode();

	const std::vector<std::string>& getAuxiliaryCommands() const;

	std::vector<std::string>& getAuxiliaryCommands();

	const Poco::SharedPtr<PTZNodeExtension>& getExtension() const;

	Poco::Optional<bool> getFixedHomePosition() const;

	Poco::Optional<bool> getGeoMove() const;

	bool getHomeSupported() const;

	int getMaximumNumberOfPresets() const;

	const Poco::Optional<std::string>& getName() const;

	const PTZSpaces& getSupportedPTZSpaces() const;

	void setAuxiliaryCommands(const std::vector<std::string>& val);

	void setAuxiliaryCommands(std::vector<std::string>&& val);

	void setExtension(const Poco::SharedPtr<PTZNodeExtension>& val);

	void setExtension(Poco::SharedPtr<PTZNodeExtension>&& val);

	void setFixedHomePosition(Poco::Optional<bool> val);

	void setGeoMove(Poco::Optional<bool> val);

	void setHomeSupported(bool val);

	void setMaximumNumberOfPresets(int val);

	void setName(const Poco::Optional<std::string>& val);

	void setName(Poco::Optional<std::string>&& val);

	void setSupportedPTZSpaces(const PTZSpaces& val);

	void setSupportedPTZSpaces(PTZSpaces&& val);

private:
	//@ mandatory=false
	//@ name=FixedHomePosition
	//@ order=0
	//@ type=attr
	Poco::Optional<bool> _fixedHomePosition;

	//@ mandatory=false
	//@ name=GeoMove
	//@ order=1
	//@ type=attr
	Poco::Optional<bool> _geoMove;

	//@ mandatory=false
	//@ name=Name
	//@ order=2
	Poco::Optional<std::string> _name;

	//@ name=SupportedPTZSpaces
	//@ order=3
	PTZSpaces _supportedPTZSpaces;

	//@ name=MaximumNumberOfPresets
	//@ order=4
	int _maximumNumberOfPresets;

	//@ name=HomeSupported
	//@ order=5
	bool _homeSupported;

	//@ mandatory=false
	//@ name=AuxiliaryCommands
	//@ order=6
	std::vector<std::string> _auxiliaryCommands;

	//@ mandatory=false
	//@ name=Extension
	//@ order=7
	Poco::SharedPtr<PTZNodeExtension> _extension;

};


inline const std::vector<std::string>& PTZNode::getAuxiliaryCommands() const
{
	return _auxiliaryCommands;
}


inline std::vector<std::string>& PTZNode::getAuxiliaryCommands()
{
	return _auxiliaryCommands;
}


inline const Poco::SharedPtr<PTZNodeExtension>& PTZNode::getExtension() const
{
	return _extension;
}


inline Poco::Optional<bool> PTZNode::getFixedHomePosition() const
{
	return _fixedHomePosition;
}


inline Poco::Optional<bool> PTZNode::getGeoMove() const
{
	return _geoMove;
}


inline bool PTZNode::getHomeSupported() const
{
	return _homeSupported;
}


inline int PTZNode::getMaximumNumberOfPresets() const
{
	return _maximumNumberOfPresets;
}


inline const Poco::Optional<std::string>& PTZNode::getName() const
{
	return _name;
}


inline const PTZSpaces& PTZNode::getSupportedPTZSpaces() const
{
	return _supportedPTZSpaces;
}


inline void PTZNode::setAuxiliaryCommands(const std::vector<std::string>& val)
{
	_auxiliaryCommands = val;
}


inline void PTZNode::setAuxiliaryCommands(std::vector<std::string>&& val)
{
	_auxiliaryCommands = std::move(val);
}


inline void PTZNode::setExtension(const Poco::SharedPtr<PTZNodeExtension>& val)
{
	_extension = val;
}


inline void PTZNode::setExtension(Poco::SharedPtr<PTZNodeExtension>&& val)
{
	_extension = std::move(val);
}


inline void PTZNode::setFixedHomePosition(Poco::Optional<bool> val)
{
	_fixedHomePosition = val;
}


inline void PTZNode::setGeoMove(Poco::Optional<bool> val)
{
	_geoMove = val;
}


inline void PTZNode::setHomeSupported(bool val)
{
	_homeSupported = val;
}


inline void PTZNode::setMaximumNumberOfPresets(int val)
{
	_maximumNumberOfPresets = val;
}


inline void PTZNode::setName(const Poco::Optional<std::string>& val)
{
	_name = val;
}


inline void PTZNode::setName(Poco::Optional<std::string>&& val)
{
	_name = std::move(val);
}


inline void PTZNode::setSupportedPTZSpaces(const PTZSpaces& val)
{
	_supportedPTZSpaces = val;
}


inline void PTZNode::setSupportedPTZSpaces(PTZSpaces&& val)
{
	_supportedPTZSpaces = std::move(val);
}


} // ONVIF


// The following headers are required for template instantiation.
#include "ONVIF/PTZNodeExtension.h"


#endif // ONVIF_PTZNode_INCLUDED
