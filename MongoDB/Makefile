#
# Makefile
#
# Makefile for Poco MongoDB
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I $(POCO_BASE)/MongoDB/include/Poco/MongoDB

objects = Array Binary Connection Database \
	Decimal128 Document Element JavaScriptCode \
	Message MessageHeader ObjectId \
	OpMsgCursor OpMsgMessage \
	PooledConnection PooledReplicaSetConnection \
	ReadPreference RegularExpression \
	ReplicaSet ReplicaSetConnection ReplicaSetURI \
	ServerDescription TopologyDescription

target         = PocoMongoDB
target_version = $(LIBVERSION)
target_libs    = PocoFoundation PocoNet

include $(POCO_BASE)/build/rules/lib
