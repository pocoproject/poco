<?xml version="1.0" encoding="utf-8"?>
<?xml-stylesheet type="text/xsl" href="../../../ver20/util/onvif-wsdl-viewer.xsl"?>
<!--
Copyright (c) 2010-2018 by ONVIF: Open Network Video Interface Forum. All rights reserved.

Recipients of this document may copy, distribute, publish, or display this
document so long as this copyright notice, license and disclaimer are
retained with all copies of the document.

THIS DOCUMENT IS PROVIDED "AS IS," AND THE CORPORATION AND ITS MEMBERS AND
THEIR AFFILIATES, MAKE NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED,
INCLUDING BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, OR TITLE;
THAT THE CONTENTS OF THIS DOCUMENT ARE SUITABLE FOR ANY PURPOSE;
OR THAT THE IMPLEMENTATION OF SUCH CONTENTS WILL NOT INFRINGE ANY PATENTS,
COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
IN NO EVENT WILL THE CORPORATION OR ITS MEMBERS OR THEIR AFFILIATES BE LIABLE
FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, PUNITIVE OR CONSEQUENTIAL
DAMAGES, ARISING OUT OF OR RELATING TO ANY USE OR DISTRIBUTION OF THIS
DOCUMENT, WHETHER OR NOT (1) THE CORPORATION, MEMBERS OR THEIR AFFILIATES
HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES, OR (2) SUCH DAMAGES
WERE REASONABLY FORESEEABLE, AND ARISING OUT OF OR RELATING TO ANY USE OR
DISTRIBUTION OF THIS DOCUMENT.
THE FOREGOING DISCLAIMER AND LIMITATION ON LIABILITY DO NOT APPLY TO,
INVALIDATE, OR LIMIT REPRESENTATIONS AND WARRANTIES MADE BY THE MEMBERS
AND THEIR RESPECTIVE AFFILIATES TO THE CORPORATION AND OTHER MEMBERS IN
CERTAIN WRITTEN POLICIES OF THE CORPORATION.
-->
<wsdl:definitions
		xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/"
		xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap12/"
		xmlns:xs="http://www.w3.org/2001/XMLSchema"
		xmlns:tar="http://www.onvif.org/ver10/accessrules/wsdl" name="AccessRulesService"
		targetNamespace="http://www.onvif.org/ver10/accessrules/wsdl">
	<!-- The data types definition for the 'Access Rules Service' -->
	<wsdl:types>
		<xs:schema targetNamespace="http://www.onvif.org/ver10/accessrules/wsdl"
				   xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:pt="http://www.onvif.org/ver10/pacs"
				   xmlns:tar="http://www.onvif.org/ver10/accessrules/wsdl" elementFormDefault="qualified" version="18.12">
			<xs:import namespace="http://www.onvif.org/ver10/pacs" schemaLocation="../../pacs/types.xsd"/>
			<!--ServiceCapabilities definitions-->
			<xs:complexType name="ServiceCapabilities">
				<xs:annotation>
					<xs:documentation>
						The service capabilities reflect optional functionality of a service. The information is static
						and does not change during device operation. The following capabilities are available:
					</xs:documentation>
				</xs:annotation>
				<xs:sequence>
					<xs:any namespace="##any" minOccurs="0" maxOccurs="unbounded" processContents="lax"/>   <!-- first ONVIF then Vendor -->
				</xs:sequence>
				<xs:attribute name="MaxLimit" use="required">
					<xs:annotation>
						<xs:documentation>
							The maximum number of entries returned by a single Get&lt;Entity&gt;List or Get&lt;Entity&gt;
							request. The device shall never return more than this number of entities in a single
							response.
						</xs:documentation>
					</xs:annotation>
					<xs:simpleType>
						<xs:restriction base="xs:unsignedInt">
							<xs:minInclusive value="1"/>
						</xs:restriction>
					</xs:simpleType>
				</xs:attribute>
				<xs:attribute name="MaxAccessProfiles" use="required">
					<xs:annotation>
						<xs:documentation>
							Indicates the maximum number of access profiles supported by the device.
						</xs:documentation>
					</xs:annotation>
					<xs:simpleType>
						<xs:restriction base="xs:unsignedInt">
							<xs:minInclusive value="1"/>
						</xs:restriction>
					</xs:simpleType>
				</xs:attribute>
				<xs:attribute name="MaxAccessPoliciesPerAccessProfile" use="required">
					<xs:annotation>
						<xs:documentation>
							Indicates the maximum number of access policies per access profile supported by the device.
						</xs:documentation>
					</xs:annotation>
					<xs:simpleType>
						<xs:restriction base="xs:unsignedInt">
							<xs:minInclusive value="1"/>
						</xs:restriction>
					</xs:simpleType>
				</xs:attribute>
				<xs:attribute name="MultipleSchedulesPerAccessPointSupported" type="xs:boolean" use="required">
					<xs:annotation>
						<xs:documentation>
							Indicates whether or not several access policies can refer to the same access point in an
							access profile.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="ClientSuppliedTokenSupported" type="xs:boolean" default="false">
					<xs:annotation>
						<xs:documentation>
							Indicates that the client is allowed to supply the token when creating access profiles. To
							enable the use of the command SetAccessProfile, the value must be set to true.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:anyAttribute processContents="lax"/>
			</xs:complexType>
			<xs:element name="Capabilities" type="tar:ServiceCapabilities"/>
			<!-- End of definition -->
			<!--AccessPolicy definitions-->
			<xs:complexType name="AccessPolicy">
				<xs:annotation>
					<xs:documentation>
						The access policy is an association of an access point and a schedule. It defines when an access
						point can be accessed using an access profile which contains this access policy. If an access
						profile contains several access policies specifying different schedules for the same access
						point will result in a union of the schedules.
					</xs:documentation>
				</xs:annotation>
				<xs:sequence>
					<xs:element name="ScheduleToken" type="pt:ReferenceToken">
						<xs:annotation>
							<xs:documentation>Reference to the schedule used by the access policy.</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Entity" type="pt:ReferenceToken">
						<xs:annotation>
							<xs:documentation>
								Reference to the entity used by the rule engine, the entity type may be specified by the
								optional EntityType field explained below but is typically an access point.
							</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="EntityType" type="xs:QName" minOccurs="0">
						<xs:annotation>
							<xs:documentation>
								Optional entity type; if missing, an access point type as defined by the ONVIF Access
								Control Service Specification should be assumed. This can also be represented by the
								QName value	“tac:AccessPoint” where tac is the namespace of ONVIF Access Control
								Service Specification. This field is provided for future extensions; it will allow an
								access policy being	extended to cover entity types other than access points as well.
							</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Extension" type="tar:AccessPolicyExtension" minOccurs="0"/>
				</xs:sequence>
				<xs:anyAttribute processContents="lax"/>
			</xs:complexType>
			<xs:complexType name="AccessPolicyExtension">
				<xs:sequence>
					<xs:any namespace="##any" processContents="lax" minOccurs="0" maxOccurs="unbounded"/>   <!-- first ONVIF then Vendor -->
				</xs:sequence>
			</xs:complexType>
			<!-- End of definition -->
			<!--AccessProfileInfo definitions-->
			<xs:complexType name="AccessProfileInfo">
				<xs:annotation>
					<xs:documentation>
						The AccessProfileInfo structure contains basic information about an access profile. The device
						shall provide the following fields for each access profile instance.
					</xs:documentation>
				</xs:annotation>
				<xs:complexContent>
					<xs:extension base="pt:DataEntity">
						<xs:sequence>
							<xs:element name="Name" type="pt:Name">
								<xs:annotation>
									<xs:documentation>A user readable name. It shall be up to 64 characters.
									</xs:documentation>
								</xs:annotation>
							</xs:element>
							<xs:element name="Description" type="pt:Description" minOccurs="0">
								<xs:annotation>
									<xs:documentation>User readable description for the access profile. It shall be up
										to 1024 characters.
									</xs:documentation>
								</xs:annotation>
							</xs:element>
						</xs:sequence>
					</xs:extension>
				</xs:complexContent>
			</xs:complexType>
			<!-- End of definition -->
			<!--AccessProfileInfo definitions-->
			<xs:complexType name="AccessProfile">
				<xs:annotation>
					<xs:documentation>
						The access profile structure contains information about the collection of access policies. The
						device shall include all properties of the AccessProfileInfo structure and also a list of access
						policies.
					</xs:documentation>
				</xs:annotation>
				<xs:complexContent>
					<xs:extension base="tar:AccessProfileInfo">
						<xs:sequence>
							<xs:element name="AccessPolicy" type="tar:AccessPolicy" minOccurs="0" maxOccurs="unbounded">
								<xs:annotation>
									<xs:documentation>A list of access policy structures, where each access policy
										defines during which schedule an access point can be accessed.
									</xs:documentation>
								</xs:annotation>
							</xs:element>
							<xs:element name="Extension" type="tar:AccessProfileExtension" minOccurs="0"/>
						</xs:sequence>
						<xs:anyAttribute processContents="lax"/>
					</xs:extension>
				</xs:complexContent>
			</xs:complexType>
			<xs:complexType name="AccessProfileExtension">
				<xs:sequence>
					<xs:any namespace="##any" processContents="lax" minOccurs="0" maxOccurs="unbounded"/>   <!-- first ONVIF then Vendor -->
				</xs:sequence>
			</xs:complexType>
			<!-- End of data types definition -->
			<!-- #################-->
			<!-- Message request / response elements-->
			<xs:element name="GetServiceCapabilities">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetServiceCapabilitiesResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Capabilities" type="tar:ServiceCapabilities">
							<xs:annotation>
								<xs:documentation>The capability response message contains the requested access rules
									service capabilities using a hierarchical XML capability structure.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetAccessProfileInfo">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken" minOccurs="1" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>Tokens of AccessProfileInfo items to get.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetAccessProfileInfoResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="AccessProfileInfo" type="tar:AccessProfileInfo" minOccurs="0"
									maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of AccessProfileInfo items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetAccessProfileInfoList">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Limit" type="xs:int" minOccurs="0">
							<xs:annotation>
								<xs:documentation>Maximum number of entries to return. If not specified, less than one
									or higher than what the device supports, the number of items is determined by the
									device.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="StartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>Start returning entries from this start reference. If not specified,
									entries shall start from the beginning of the dataset.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetAccessProfileInfoListResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="NextStartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>StartReference to use in next call to get the following items. If
									absent, no more items to get.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="AccessProfileInfo" type="tar:AccessProfileInfo" minOccurs="0"
									maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of AccessProfileInfo items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetAccessProfiles">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken" minOccurs="1" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>Tokens of AccessProfile items to get.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetAccessProfilesResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="AccessProfile" type="tar:AccessProfile" minOccurs="0" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of Access Profile items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetAccessProfileList">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Limit" type="xs:int" minOccurs="0">
							<xs:annotation>
								<xs:documentation>Maximum number of entries to return. If not specified, less than one
									or higher than what the device supports, the number of items is determined by the
									device.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="StartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>Start returning entries from this start reference. If not specified,
									entries shall start from the beginning of the dataset.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetAccessProfileListResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="NextStartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>StartReference to use in next call to get the following items. If
									absent, no more items to get.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="AccessProfile" type="tar:AccessProfile" minOccurs="0" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of Access Profile items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="CreateAccessProfile">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="AccessProfile" type="tar:AccessProfile">
							<xs:annotation>
								<xs:documentation>The AccessProfile to create.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="CreateAccessProfileResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The Token of created AccessProfile.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="ModifyAccessProfile">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="AccessProfile" type="tar:AccessProfile">
							<xs:annotation>
								<xs:documentation>The details of Access Profile</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="ModifyAccessProfileResponse">
				<xs:complexType>
					<xs:sequence>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->			
			<xs:element name="SetAccessProfile">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="AccessProfile" type="tar:AccessProfile">
							<xs:annotation>
								<xs:documentation>The AccessProfile item to create or modify</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="SetAccessProfileResponse">
				<xs:complexType>
					<xs:sequence>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="DeleteAccessProfile">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The token of the access profile to delete.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="DeleteAccessProfileResponse">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<!-- #################-->
		</xs:schema>
	</wsdl:types>
	<!--Definition of 'GetServiceCapabilities' message-->
	<wsdl:message name="GetServiceCapabilitiesRequest">
		<wsdl:part name="parameters" element="tar:GetServiceCapabilities"/>
	</wsdl:message>
	<wsdl:message name="GetServiceCapabilitiesResponse">
		<wsdl:part name="parameters" element="tar:GetServiceCapabilitiesResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetAccessProfileInfo' message-->
	<wsdl:message name="GetAccessProfileInfoRequest">
		<wsdl:part name="parameters" element="tar:GetAccessProfileInfo"/>
	</wsdl:message>
	<wsdl:message name="GetAccessProfileInfoResponse">
		<wsdl:part name="parameters" element="tar:GetAccessProfileInfoResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetAccessProfileInfoList' message-->
	<wsdl:message name="GetAccessProfileInfoListRequest">
		<wsdl:part name="parameters" element="tar:GetAccessProfileInfoList"/>
	</wsdl:message>
	<wsdl:message name="GetAccessProfileInfoListResponse">
		<wsdl:part name="parameters" element="tar:GetAccessProfileInfoListResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetAccessProfiles' message-->
	<wsdl:message name="GetAccessProfilesRequest">
		<wsdl:part name="parameters" element="tar:GetAccessProfiles"/>
	</wsdl:message>
	<wsdl:message name="GetAccessProfilesResponse">
		<wsdl:part name="parameters" element="tar:GetAccessProfilesResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetAccessProfileList' message-->
	<wsdl:message name="GetAccessProfileListRequest">
		<wsdl:part name="parameters" element="tar:GetAccessProfileList"/>
	</wsdl:message>
	<wsdl:message name="GetAccessProfileListResponse">
		<wsdl:part name="parameters" element="tar:GetAccessProfileListResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'CreateAccessProfile' message-->
	<wsdl:message name="CreateAccessProfileRequest">
		<wsdl:part name="parameters" element="tar:CreateAccessProfile"/>
	</wsdl:message>
	<wsdl:message name="CreateAccessProfileResponse">
		<wsdl:part name="parameters" element="tar:CreateAccessProfileResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'ModifyAccessProfile' message-->
	<wsdl:message name="ModifyAccessProfileRequest">
		<wsdl:part name="parameters" element="tar:ModifyAccessProfile"/>
	</wsdl:message>
	<wsdl:message name="ModifyAccessProfileResponse">
		<wsdl:part name="parameters" element="tar:ModifyAccessProfileResponse"/>
	</wsdl:message>
  <!--Definition of 'SetAccessProfile' message-->
	<wsdl:message name="SetAccessProfileRequest">
		<wsdl:part name="parameters" element="tar:SetAccessProfile"/>
	</wsdl:message>
	<wsdl:message name="SetAccessProfileResponse">
		<wsdl:part name="parameters" element="tar:SetAccessProfileResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'DeleteAccessProfile' message-->
	<wsdl:message name="DeleteAccessProfileRequest">
		<wsdl:part name="parameters" element="tar:DeleteAccessProfile"/>
	</wsdl:message>
	<wsdl:message name="DeleteAccessProfileResponse">
		<wsdl:part name="parameters" element="tar:DeleteAccessProfileResponse"/>
	</wsdl:message>
	<!--===============================-->
	<wsdl:portType name="AccessRulesPort">
		<wsdl:operation name="GetServiceCapabilities">
			<wsdl:documentation>This operation returns the capabilities of the access rules service.
			</wsdl:documentation>
			<wsdl:input message="tar:GetServiceCapabilitiesRequest"/>
			<wsdl:output message="tar:GetServiceCapabilitiesResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetAccessProfileInfo">
			<wsdl:documentation>
				This operation requests a list of AccessProfileInfo items matching the given tokens. The device shall
				ignore tokens it cannot resolve and shall return an empty list if there are no items matching the
				specified tokens. The device shall not return a fault in this case.
				If the number of requested items is greater than MaxLimit, a TooManyItems fault shall be returned.
			</wsdl:documentation>
			<wsdl:input message="tar:GetAccessProfileInfoRequest"/>
			<wsdl:output message="tar:GetAccessProfileInfoResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetAccessProfileInfoList">
			<wsdl:documentation>
				This operation requests a list of all of AccessProfileInfo items provided by the device.
				A call to this method shall return a StartReference when not all data is returned and more data is
				available. The reference shall be valid for retrieving the next set of data.
				The number of items returned shall not be greater than the Limit parameter.
			</wsdl:documentation>
			<wsdl:input message="tar:GetAccessProfileInfoListRequest"/>
			<wsdl:output message="tar:GetAccessProfileInfoListResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetAccessProfiles">
			<wsdl:documentation>
				This operation returns the specified access profile item matching the given tokens.
				The device shall ignore tokens it cannot resolve and shall return an empty list if there are no items
				matching specified tokens. The device shall not return a fault in this case.
				If the number of requested items is greater than MaxLimit, a TooManyItems fault shall be returned.
			</wsdl:documentation>
			<wsdl:input message="tar:GetAccessProfilesRequest"/>
			<wsdl:output message="tar:GetAccessProfilesResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetAccessProfileList">
			<wsdl:documentation>
				This operation requests a list of all of access profile items provided by the device.
				A call to this method shall return a StartReference when not all data is returned and more data is
				available. The reference shall be valid for retrieving the next set of data. 
				The number of items returned shall not be greater than the Limit parameter.
			</wsdl:documentation>
			<wsdl:input message="tar:GetAccessProfileListRequest"/>
			<wsdl:output message="tar:GetAccessProfileListResponse"/>
		</wsdl:operation>
		<wsdl:operation name="CreateAccessProfile">
			<wsdl:documentation>
				This operation creates the specified access profile in the device. The token field of the access profile shall be
				empty, the service shall allocate a token for the access profile. The allocated token shall be returned
				in the response. If the client sends any value in the token field, the device shall return InvalidArgVal
				as generic fault code.
				In an access profile, if several access policies specifying different schedules for the same access
				point will result in a union of the schedules.
			</wsdl:documentation>
			<wsdl:input message="tar:CreateAccessProfileRequest"/>
			<wsdl:output message="tar:CreateAccessProfileResponse"/>
		</wsdl:operation>
		<wsdl:operation name="ModifyAccessProfile">
			<wsdl:documentation>
				This operation will modify the access profile for the specified access profile token. The token of the
				access profile to modify is specified in the token field of the AccessProile structure and shall not
				be empty. All other fields in the structure shall overwrite the fields in the specified access profile.
				If several access policies specifying different schedules for the same access point will result in a
				union of the schedules.
				If the device could not store the access profile information then a fault will be generated.
			</wsdl:documentation>
			<wsdl:input message="tar:ModifyAccessProfileRequest"/>
			<wsdl:output message="tar:ModifyAccessProfileResponse"/>
		</wsdl:operation>
		<wsdl:operation name="SetAccessProfile">
			<wsdl:documentation>
				This operation will synchronize an access profile in a client with the device.
				If an access profile with the specified token does not exist in the device, the access profile is
				created. If an access profile with the specified token exists, then the access profile is modified.
				A call to this method takes an access profile structure as input parameter. The token field of the
				access profile must not be empty.
				A device that signals support for the ClientSuppliedTokenSupported capability shall implement this command.
			</wsdl:documentation>
			<wsdl:input message="tar:SetAccessProfileRequest"/>
			<wsdl:output message="tar:SetAccessProfileResponse"/>
		</wsdl:operation>
		<wsdl:operation name="DeleteAccessProfile">
			<wsdl:documentation>
				This operation will delete the specified access profile.
				If the access profile is deleted, all access policies associated to the access profile will also be
				deleted.
				If it is associated with one or more entities some devices may not be able to delete the access profile,
				and consequently a ReferenceInUse fault shall be generated.
			</wsdl:documentation>
			<wsdl:input message="tar:DeleteAccessProfileRequest"/>
			<wsdl:output message="tar:DeleteAccessProfileResponse"/>
		</wsdl:operation>
	</wsdl:portType>
	<!--Protocol & data format for the operations and messages for the port type 'AccessRulesPort'-->
	<wsdl:binding name="AccessRulesBinding" type="tar:AccessRulesPort">
		<soap:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/>
		<wsdl:operation name="GetServiceCapabilities">
			<soap:operation soapAction="http://www.onvif.org/ver10/accessrules/wsdl/GetServiceCapabilities"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetAccessProfileInfo">
			<soap:operation soapAction="http://www.onvif.org/ver10/accessrules/wsdl/GetAccessProfileInfo"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetAccessProfileInfoList">
			<soap:operation soapAction="http://www.onvif.org/ver10/accessrules/wsdl/GetAccessProfileInfoList"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetAccessProfiles">
			<soap:operation soapAction="http://www.onvif.org/ver10/accessrules/wsdl/GetAccessProfiles"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetAccessProfileList">
			<soap:operation soapAction="http://www.onvif.org/ver10/accessrules/wsdl/GetAccessProfileList"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="CreateAccessProfile">
			<soap:operation soapAction="http://www.onvif.org/ver10/accessrules/wsdl/CreateAccessProfile"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="ModifyAccessProfile">
			<soap:operation soapAction="http://www.onvif.org/ver10/accessrules/wsdl/ModifyAccessProfile"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
    <wsdl:operation name="SetAccessProfile">
			<soap:operation soapAction="http://www.onvif.org/ver10/accessrules/wsdl/SetAccessProfile"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="DeleteAccessProfile">
			<soap:operation soapAction="http://www.onvif.org/ver10/accessrules/wsdl/DeleteAccessProfile"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
	</wsdl:binding>
</wsdl:definitions>
