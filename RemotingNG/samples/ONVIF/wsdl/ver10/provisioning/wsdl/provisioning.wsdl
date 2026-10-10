<?xml version="1.0" encoding="UTF-8"?>
<?xml-stylesheet type="text/xsl" href="../../../ver20/util/onvif-wsdl-viewer.xsl"?>
<!--
	Copyright (c) 2018 by ONVIF: Open Network Video Interface Forum. All rights reserved.
	
	Recipients of this document may copy, distribute, publish, or display this document so long as this copyright notice, license and disclaimer are retained with all copies of the document. No license is granted to modify this document.
	
	THIS DOCUMENT IS PROVIDED "AS IS," AND THE CORPORATION AND ITS MEMBERS AND THEIR AFFILIATES, MAKE NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO, WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT, OR TITLE; THAT THE CONTENTS OF THIS DOCUMENT ARE SUITABLE FOR ANY PURPOSE; OR THAT THE IMPLEMENTATION OF SUCH CONTENTS WILL NOT INFRINGE ANY PATENTS, COPYRIGHTS, TRADEMARKS OR OTHER RIGHTS.
	IN NO EVENT WILL THE CORPORATION OR ITS MEMBERS OR THEIR AFFILIATES BE LIABLE FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL, PUNITIVE OR CONSEQUENTIAL DAMAGES, ARISING OUT OF OR RELATING TO ANY USE OR DISTRIBUTION OF THIS DOCUMENT, WHETHER OR NOT (1) THE CORPORATION, MEMBERS OR THEIR AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES, OR (2) SUCH DAMAGES WERE REASONABLY FORESEEABLE, AND ARISING OUT OF OR RELATING TO ANY USE OR DISTRIBUTION OF THIS DOCUMENT.  THE FOREGOING DISCLAIMER AND LIMITATION ON LIABILITY DO NOT APPLY TO, INVALIDATE, OR LIMIT REPRESENTATIONS AND WARRANTIES MADE BY THE MEMBERS AND THEIR RESPECTIVE AFFILIATES TO THE CORPORATION AND OTHER MEMBERS IN CERTAIN WRITTEN POLICIES OF THE CORPORATION.
-->
<wsdl:definitions xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/" xmlns:tpv="http://www.onvif.org/ver10/provisioning/wsdl" xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap12/" targetNamespace="http://www.onvif.org/ver10/provisioning/wsdl">
	<wsdl:types>
		<xs:schema xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:tt="http://www.onvif.org/ver10/schema" elementFormDefault="qualified" targetNamespace="http://www.onvif.org/ver10/provisioning/wsdl" version="18.12">
			<xs:import namespace="http://www.onvif.org/ver10/schema" schemaLocation="../../schema/onvif.xsd"/>
			<!--===================================================-->
			<!-- Data types used by the provisioning service -->
			<!--===================================================-->
			<xs:simpleType name="PanDirection">
				<xs:annotation>
					<xs:documentation>The direction for PanMove to move the device.</xs:documentation>
				</xs:annotation>
				<xs:restriction base="xs:string">
					<xs:enumeration value="Left">
						<xs:annotation>
							<xs:documentation>Move left in relation to the video source image.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
					<xs:enumeration value="Right">
						<xs:annotation>
							<xs:documentation>Move right in relation to the video source image.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
				</xs:restriction>
			</xs:simpleType>
			<!--===============================-->
			<xs:simpleType name="TiltDirection">
				<xs:annotation>
					<xs:documentation>The direction for TiltMove to move the device.</xs:documentation>
				</xs:annotation>
				<xs:restriction base="xs:string">
					<xs:enumeration value="Up">
						<xs:annotation>
							<xs:documentation>Move up in relation to the video source image.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
					<xs:enumeration value="Down">
						<xs:annotation>
							<xs:documentation>Move down in relation to the video source image.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
				</xs:restriction>
			</xs:simpleType>
			<!--===============================-->
			<xs:simpleType name="ZoomDirection">
				<xs:annotation>
					<xs:documentation>The direction for ZoomMove to change the focal length in relation to the video source.</xs:documentation>
				</xs:annotation>
				<xs:restriction base="xs:string">
					<xs:enumeration value="Wide">
						<xs:annotation>
							<xs:documentation>Move video source lens toward a wider field of view.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
					<xs:enumeration value="Telephoto">
						<xs:annotation>
							<xs:documentation>Move video source lens toward a narrower field of view.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
				</xs:restriction>
			</xs:simpleType>
			<!--===============================-->
			<xs:simpleType name="RollDirection">
				<xs:annotation>
					<xs:documentation>The direction for RollMove to move the device.</xs:documentation>
				</xs:annotation>
				<xs:restriction base="xs:string">
					<xs:enumeration value="Clockwise">
						<xs:annotation>
							<xs:documentation>Move clockwise in relation to the video source image.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
					<xs:enumeration value="Counterclockwise">
						<xs:annotation>
							<xs:documentation>Move counterclockwise in relation to the video source image.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
					<xs:enumeration value="Auto">
						<xs:annotation>
							<xs:documentation>Automatically level the device in relation to the video source image.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
				</xs:restriction>
			</xs:simpleType>
			<!--===============================-->
			<xs:simpleType name="FocusDirection">
				<xs:annotation>
					<xs:documentation>The direction for FocusMove to move the focal plane in relation to the video source.</xs:documentation>
				</xs:annotation>
				<xs:restriction base="xs:string">
					<xs:enumeration value="Near">
						<xs:annotation>
							<xs:documentation>Move to focus on close objects.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
					<xs:enumeration value="Far">
						<xs:annotation>
							<xs:documentation>Move to focus on distant objects.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
					<xs:enumeration value="Auto">
						<xs:annotation>
							<xs:documentation>Automatically focus for the sharpest video source image.</xs:documentation>
						</xs:annotation>
					</xs:enumeration>
				</xs:restriction>
			</xs:simpleType>
			<!--===============================-->
			<xs:complexType name="Usage">
				<xs:annotation>
					<xs:documentation>The quantity of movement events that have occured over the lifetime of the device.</xs:documentation>
				</xs:annotation>
				<xs:sequence>
					<xs:element name="Pan" type="xs:positiveInteger" minOccurs="0">
						<xs:annotation>
							<xs:documentation>The quantity of pan movement events over the life of the device.</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Tilt" type="xs:positiveInteger" minOccurs="0">
						<xs:annotation>
							<xs:documentation>The quantity of tilt movement events over the life of the device.</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Zoom" type="xs:positiveInteger" minOccurs="0">
						<xs:annotation>
							<xs:documentation>The quantity of zoom movement events over the life of the device.</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Roll" type="xs:positiveInteger" minOccurs="0">
						<xs:annotation>
							<xs:documentation>The quantity of roll movement events over the life of the device.</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Focus" type="xs:positiveInteger" minOccurs="0">
						<xs:annotation>
							<xs:documentation>The quantity of focus movement events over the life of the device.</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:any namespace="##any" processContents="lax" minOccurs="0" maxOccurs="unbounded"/>
					<!-- first ONVIF then vendor -->
				</xs:sequence>
				<xs:anyAttribute processContents="lax"/>
			</xs:complexType>
			<!--===============================-->
			<xs:complexType name="SourceCapabilities">
				<xs:annotation>
					<xs:documentation>The provisioning capabilities of a video source on the device.</xs:documentation>
				</xs:annotation>
				<xs:sequence>
					<xs:any namespace="##any" minOccurs="0" maxOccurs="unbounded" processContents="lax"/>
					<!-- first ONVIF then vendor -->
				</xs:sequence>
				<xs:attribute name="VideoSourceToken" type="tt:ReferenceToken" use="required">
					<xs:annotation>
						<xs:documentation>Unique identifier of a video source.</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaximumPanMoves" type="xs:positiveInteger">
					<xs:annotation>
						<xs:documentation>Lifetime limit of pan moves for this video source.  Presence of this attribute indicates support of pan move.</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaximumTiltMoves" type="xs:positiveInteger">
					<xs:annotation>
						<xs:documentation>Lifetime limit of tilt moves for this video source.  Presence of this attribute indicates support of tilt move.</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaximumZoomMoves" type="xs:positiveInteger">
					<xs:annotation>
						<xs:documentation>Lifetime limit of zoom moves for this video source.  Presence of this attribute indicates support of zoom move.</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaximumRollMoves" type="xs:positiveInteger">
					<xs:annotation>
						<xs:documentation>Lifetime limit of roll moves for this video source.  Presence of this attribute indicates support of roll move.</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="AutoLevel" type="xs:boolean">
					<xs:annotation>
						<xs:documentation>Indicates "auto" as a valid enum for Direction in RollMove.</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaximumFocusMoves" type="xs:positiveInteger">
					<xs:annotation>
						<xs:documentation>Lifetime limit of focus moves for this video source.  Presence of this attribute indicates support of focus move.</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="AutoFocus" type="xs:boolean">
					<xs:annotation>
						<xs:documentation>Indicates "auto" as a valid enum for Direction in FocusMove.</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:anyAttribute processContents="lax"/>
			</xs:complexType>
			<!--===============================-->
			<xs:complexType name="Capabilities">
				<xs:annotation>
					<xs:documentation>The capabilities of Provisioning Service on the device.</xs:documentation>
				</xs:annotation>
				<xs:sequence>
					<xs:element name="DefaultTimeout" type="xs:duration">
						<xs:annotation>
							<xs:documentation>Maximum time before stopping movement after a move operation.</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Source" type="tpv:SourceCapabilities" minOccurs="0" maxOccurs="unbounded">
						<xs:annotation>
							<xs:documentation>Capabilities per video source.</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:any namespace="##any" processContents="lax" minOccurs="0" maxOccurs="unbounded"/>
					<!-- first ONVIF then vendor -->
				</xs:sequence>
				<xs:anyAttribute processContents="lax"/>
			</xs:complexType>
			<xs:element name="Capabilities" type="tpv:Capabilities"/>
			<!--=========================================-->
			<!-- Request/response elements               -->
			<!--=========================================-->
			<xs:element name="GetServiceCapabilities">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<xs:element name="GetServiceCapabilitiesResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Capabilities" type="tpv:Capabilities">
							<xs:annotation>
								<xs:documentation>The capabilities for the provisioning service on this device.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="PanMove">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="VideoSource" type="tt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The video source associated with the provisioning.</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Direction" type="tpv:PanDirection">
							<xs:annotation>
								<xs:documentation>"left" or "right".</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Timeout" type="xs:duration" minOccurs="0">
							<xs:annotation>
								<xs:documentation>"Operation timeout, if less than default timeout.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<xs:element name="PanMoveResponse">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="TiltMove">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="VideoSource" type="tt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The video source associated with the provisioning.</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Direction" type="tpv:TiltDirection">
							<xs:annotation>
								<xs:documentation>"up" or "down".</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Timeout" type="xs:duration" minOccurs="0">
							<xs:annotation>
								<xs:documentation>"Operation timeout, if less than default timeout.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<xs:element name="TiltMoveResponse">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="ZoomMove">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="VideoSource" type="tt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The video source associated with the provisioning.</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Direction" type="tpv:ZoomDirection">
							<xs:annotation>
								<xs:documentation>"wide" or "telephoto".</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Timeout" type="xs:duration" minOccurs="0">
							<xs:annotation>
								<xs:documentation>"Operation timeout, if less than default timeout.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<xs:element name="ZoomMoveResponse">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="RollMove">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="VideoSource" type="tt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The video source associated with the provisioning.</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Direction" type="tpv:RollDirection">
							<xs:annotation>
								<xs:documentation>"clockwise", "counterclockwise", or "auto".</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Timeout" type="xs:duration" minOccurs="0">
							<xs:annotation>
								<xs:documentation>"Operation timeout, if less than default timeout.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<xs:element name="RollMoveResponse">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="FocusMove">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="VideoSource" type="tt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The video source associated with the provisioning.</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Direction" type="tpv:FocusDirection">
							<xs:annotation>
								<xs:documentation>"near", "far", or "auto".</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Timeout" type="xs:duration" minOccurs="0">
							<xs:annotation>
								<xs:documentation>"Operation timeout, if less than default timeout.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<xs:element name="FocusMoveResponse">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="Stop">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="VideoSource" type="tt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The video source associated with the provisioning.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<xs:element name="StopResponse">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetUsage">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="VideoSource" type="tt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The video source associated with the provisioning.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<xs:element name="GetUsageResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Usage" type="tpv:Usage">
							<xs:annotation>
								<xs:documentation>The set of lifetime usage values for the provisioning associated with the video source.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
		</xs:schema>
	</wsdl:types>
	<wsdl:message name="GetServiceCapabilitiesRequest">
		<wsdl:part name="parameters" element="tpv:GetServiceCapabilities"/>
	</wsdl:message>
	<wsdl:message name="GetServiceCapabilitiesResponse">
		<wsdl:part name="parameters" element="tpv:GetServiceCapabilitiesResponse"/>
	</wsdl:message>
	<wsdl:message name="PanMoveRequest">
		<wsdl:part name="parameters" element="tpv:PanMove"/>
	</wsdl:message>
	<wsdl:message name="PanMoveResponse">
		<wsdl:part name="parameters" element="tpv:PanMoveResponse"/>
	</wsdl:message>
	<wsdl:message name="TiltMoveRequest">
		<wsdl:part name="parameters" element="tpv:TiltMove"/>
	</wsdl:message>
	<wsdl:message name="TiltMoveResponse">
		<wsdl:part name="parameters" element="tpv:TiltMoveResponse"/>
	</wsdl:message>
	<wsdl:message name="ZoomMoveRequest">
		<wsdl:part name="parameters" element="tpv:ZoomMove"/>
	</wsdl:message>
	<wsdl:message name="ZoomMoveResponse">
		<wsdl:part name="parameters" element="tpv:ZoomMoveResponse"/>
	</wsdl:message>
	<wsdl:message name="RollMoveRequest">
		<wsdl:part name="parameters" element="tpv:RollMove"/>
	</wsdl:message>
	<wsdl:message name="RollMoveResponse">
		<wsdl:part name="parameters" element="tpv:RollMoveResponse"/>
	</wsdl:message>
	<wsdl:message name="FocusMoveRequest">
		<wsdl:part name="parameters" element="tpv:FocusMove"/>
	</wsdl:message>
	<wsdl:message name="FocusMoveResponse">
		<wsdl:part name="parameters" element="tpv:FocusMoveResponse"/>
	</wsdl:message>
	<wsdl:message name="StopRequest">
		<wsdl:part name="parameters" element="tpv:Stop"/>
	</wsdl:message>
	<wsdl:message name="StopResponse">
		<wsdl:part name="parameters" element="tpv:StopResponse"/>
	</wsdl:message>
	<wsdl:message name="GetUsageRequest">
		<wsdl:part name="parameters" element="tpv:GetUsage"/>
	</wsdl:message>
	<wsdl:message name="GetUsageResponse">
		<wsdl:part name="parameters" element="tpv:GetUsageResponse"/>
	</wsdl:message>
	<wsdl:portType name="ProvisioningService">
		<wsdl:documentation>Functionality for all provisioning service operations.</wsdl:documentation>
		<wsdl:operation name="GetServiceCapabilities">
			<wsdl:documentation>Returns the capabilities of the provisioning service.</wsdl:documentation>
			<wsdl:input message="tpv:GetServiceCapabilitiesRequest"/>
			<wsdl:output message="tpv:GetServiceCapabilitiesResponse"/>
		</wsdl:operation>
		<wsdl:operation name="PanMove">
			<wsdl:documentation>Moves device on the pan axis.</wsdl:documentation>
			<wsdl:input message="tpv:PanMoveRequest"/>
			<wsdl:output message="tpv:PanMoveResponse"/>
		</wsdl:operation>
		<wsdl:operation name="TiltMove">
			<wsdl:documentation>Moves device on the tilt axis.</wsdl:documentation>
			<wsdl:input message="tpv:TiltMoveRequest"/>
			<wsdl:output message="tpv:TiltMoveResponse"/>
		</wsdl:operation>
		<wsdl:operation name="ZoomMove">
			<wsdl:documentation>Moves device on the zoom axis.</wsdl:documentation>
			<wsdl:input message="tpv:ZoomMoveRequest"/>
			<wsdl:output message="tpv:ZoomMoveResponse"/>
		</wsdl:operation>
		<wsdl:operation name="RollMove">
			<wsdl:documentation>Moves device on the roll axis.</wsdl:documentation>
			<wsdl:input message="tpv:RollMoveRequest"/>
			<wsdl:output message="tpv:RollMoveResponse"/>
		</wsdl:operation>
		<wsdl:operation name="FocusMove">
			<wsdl:documentation>Moves device on the focus axis.</wsdl:documentation>
			<wsdl:input message="tpv:FocusMoveRequest"/>
			<wsdl:output message="tpv:FocusMoveResponse"/>
		</wsdl:operation>
		<wsdl:operation name="Stop">
			<wsdl:documentation>Stops device motion on all axes.</wsdl:documentation>
			<wsdl:input message="tpv:StopRequest"/>
			<wsdl:output message="tpv:StopResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetUsage">
			<wsdl:documentation>Returns the lifetime move counts.</wsdl:documentation>
			<wsdl:input message="tpv:GetUsageRequest"/>
			<wsdl:output message="tpv:GetUsageResponse"/>
		</wsdl:operation>
	</wsdl:portType>
	<wsdl:binding name="ProvisioningBinding" type="tpv:ProvisioningService">
		<soap:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/>
		<wsdl:operation name="GetServiceCapabilities">
			<soap:operation soapAction="http://www.onvif.org/ver10/provisioning/wsdl/GetServiceCapabilities"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="PanMove">
			<soap:operation soapAction="http://www.onvif.org/ver10/provisioning/wsdl/PanMove"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="TiltMove">
			<soap:operation soapAction="http://www.onvif.org/ver10/provisioning/wsdl/TiltMove"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="ZoomMove">
			<soap:operation soapAction="http://www.onvif.org/ver10/provisioning/wsdl/ZoomMove"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="RollMove">
			<soap:operation soapAction="http://www.onvif.org/ver10/provisioning/wsdl/RollMove"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="FocusMove">
			<soap:operation soapAction="http://www.onvif.org/ver10/provisioning/wsdl/FocusMove"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="Stop">
			<soap:operation soapAction="http://www.onvif.org/ver10/provisioning/wsdl/Stop"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetUsage">
			<soap:operation soapAction="http://www.onvif.org/ver10/provisioning/wsdl/Usage"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
	</wsdl:binding>
</wsdl:definitions>
