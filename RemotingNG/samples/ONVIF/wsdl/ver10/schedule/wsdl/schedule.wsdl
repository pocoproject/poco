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
		xmlns:tsc="http://www.onvif.org/ver10/schedule/wsdl" name="ScheduleService"
		targetNamespace="http://www.onvif.org/ver10/schedule/wsdl">
	<!-- The data types definition for the 'Schedule Service' -->
	<wsdl:types>
		<xs:schema targetNamespace="http://www.onvif.org/ver10/schedule/wsdl"
				   xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:pt="http://www.onvif.org/ver10/pacs"
				   xmlns:tsc="http://www.onvif.org/ver10/schedule/wsdl" elementFormDefault="qualified" version="18.12">
			<xs:import namespace="http://www.onvif.org/ver10/pacs" schemaLocation="../../pacs/types.xsd"/>
			<!--ServiceCapabilities definitions-->
			<xs:complexType name="ServiceCapabilities">
				<xs:annotation>
					<xs:documentation>
						The service capabilities reflect optional functionality of a service.
						The information is static and does not change during device operation.
						The following capabilities are available:
					</xs:documentation>
				</xs:annotation>
				<xs:sequence>
					<xs:any namespace="##any" minOccurs="0" maxOccurs="unbounded" processContents="lax"/>   <!-- first ONVIF then Vendor -->
				</xs:sequence>
				<xs:attribute name="MaxLimit" type="pt:PositiveInteger" use="required">
					<xs:annotation>
						<xs:documentation>
							The maximum number of entries returned by a single Get&lt;Entity&gt;List or
							Get&lt;Entity&gt; request. The device shall never return more than this number
							of entities in a single response.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaxSchedules" type="pt:PositiveInteger" use="required">
					<xs:annotation>
						<xs:documentation>
							Indicates the maximum number of schedules the device supports.
							The device shall support at least one schedule.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaxTimePeriodsPerDay" type="pt:PositiveInteger" use="required">
					<xs:annotation>
						<xs:documentation>
							Indicates the maximum number of time periods per day the device supports
							in a schedule including special days schedule. The device shall support
							at least one time period per day.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaxSpecialDayGroups" type="pt:PositiveInteger" use="required">
					<xs:annotation>
						<xs:documentation>
							Indicates the maximum number of special day group entities the device supports.
							The device shall support at least one ‘SpecialDayGroup’ entity.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaxDaysInSpecialDayGroup" type="pt:PositiveInteger" use="required">
					<xs:annotation>
						<xs:documentation>
							Indicates the maximum number of days per ‘SpecialDayGroup’ entity the device
							supports. The device shall support at least one day per ‘SpecialDayGroup’ entity.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="MaxSpecialDaysSchedules" type="pt:PositiveInteger" use="required">
					<xs:annotation>
						<xs:documentation>
							Indicates the maximum number of ‘SpecialDaysSchedule’ entities referred by a
							schedule that the device supports.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="ExtendedRecurrenceSupported" type="xs:boolean" use="required">
					<xs:annotation>
						<xs:documentation>
							For schedules:
							If this capability is supported, then all iCalendar recurrence types shall
							be supported by the device. The device shall also support the start and end dates (or
							iCalendar occurrence count) in recurring events (see iCalendar examples in section 3).
							If this capability is not supported, then only the weekly iCalendar recurrence
							type shall be supported. Non-recurring events and other recurring types are
							not supported. The device shall only accept a start date with the year ‘1970’
							(the month and day is needed to reflect the week day of the recurrence)
							and will not accept an occurrence count (or iCalendar until date) in recurring events.
							For special days (only applicable if SpecialDaysSupported is set to true):
							If this capability is supported, then all iCalendar recurrence types shall
							be supported by the device. The device shall also support the start and
							end dates (or occurrence count) in recurring events.
							If this capability is not supported, then only non-recurring special days are supported.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="SpecialDaysSupported" type="xs:boolean" use="required">
					<xs:annotation>
						<xs:documentation>
							If this capability is supported, then the device shall support special days.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="StateReportingSupported" type="xs:boolean" use="required">
					<xs:annotation>
						<xs:documentation>
							If this capability is set to true, the device shall implement the
							GetScheduleState command, and shall notify subscribing clients whenever
							schedules become active or inactive.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:attribute name="ClientSuppliedTokenSupported" type="xs:boolean" default="false">
					<xs:annotation>
						<xs:documentation>
							Indicates that the client is allowed to supply the token when creating schedules and special day groups.
							To enable the use of the commands SetSchedule and SetSpecialDayGroup, the value must be set to true.
						</xs:documentation>
					</xs:annotation>
				</xs:attribute>
				<xs:anyAttribute processContents="lax"/>
			</xs:complexType>
			<xs:element name="Capabilities" type="tsc:ServiceCapabilities"/>
			<!-- End of definition -->
			<!--ScheduleInfo definitions-->
			<xs:complexType name="ScheduleInfo">
				<xs:annotation>
					<xs:documentation>
						The ScheduleInfo type represents the schedule as a physical object.
						The structure contains information of a specific schedule instance.
					</xs:documentation>
				</xs:annotation>
				<xs:complexContent>
					<xs:extension base="pt:DataEntity">
						<xs:sequence>
							<xs:element name="Name" type="pt:Name">
								<xs:annotation>
									<xs:documentation>
										A user readable name. It shall be up to 64 characters.
									</xs:documentation>
								</xs:annotation>
							</xs:element>
							<xs:element name="Description" type="pt:Description" minOccurs="0">
								<xs:annotation>
									<xs:documentation>
										User readable description for the schedule. It shall be up to 1024 characters.
									</xs:documentation>
								</xs:annotation>
							</xs:element>
						</xs:sequence>
					</xs:extension>
				</xs:complexContent>
			</xs:complexType>
			<!-- End of definition -->
			<!--Schedule definitions-->
			<xs:complexType name="Schedule">
				<xs:annotation>
					<xs:documentation>
						The schedule structure shall include all properties of the ScheduleInfo structure
						and also the standard events (iCalendar format) and a list of SpecialDaysSchedule instances.
					</xs:documentation>
				</xs:annotation>
				<xs:complexContent>
					<xs:extension base="tsc:ScheduleInfo">
						<xs:sequence>
							<xs:element name="Standard" type="xs:string">
								<xs:annotation>
									<xs:documentation>
										An iCalendar structure that defines a number of events. Events
										can be recurring or non-recurring. The events can, for instance,
										be used to control when a camera should record or when a facility
										is accessible. Some devices might not be able to fully support
										all the features of iCalendar. Setting the service capability
										ExtendedRecurrenceSupported to false will enable more devices
										to be ONVIF compliant. Is of type string (but contains an iCalendar structure).
									</xs:documentation>
								</xs:annotation>
							</xs:element>
							<xs:element name="SpecialDays" type="tsc:SpecialDaysSchedule" minOccurs="0"
										maxOccurs="unbounded">
								<xs:annotation>
									<xs:documentation>
										For devices that are not able to support all the features of iCalendar,
										supporting special days is essential. Each SpecialDaysSchedule
										instance defines an alternate set of time periods that overrides
										the regular schedule for a specified list of special days.
										Is of type SpecialDaysSchedule.
									</xs:documentation>
								</xs:annotation>
							</xs:element>
							<xs:element name="Extension" type="tsc:ScheduleExtension" minOccurs="0"/>
						</xs:sequence>
						<xs:anyAttribute processContents="lax"/>
					</xs:extension>
				</xs:complexContent>
			</xs:complexType>
			<xs:complexType name="ScheduleExtension">
				<xs:sequence>
					<xs:any namespace="##any" processContents="lax" minOccurs="0" maxOccurs="unbounded"/>   <!-- first ONVIF then Vendor -->
				</xs:sequence>
			</xs:complexType>
			<!-- End of definition -->
			<!--SpecialDaysSchedule definitions-->
			<xs:complexType name="SpecialDaysSchedule">
				<xs:annotation>
					<xs:documentation>
						A override schedule that defines alternate time periods for a group of special days.
					</xs:documentation>
				</xs:annotation>
				<xs:sequence>
					<xs:element name="GroupToken" type="pt:ReferenceToken">
						<xs:annotation>
							<xs:documentation>
								Indicates the list of special days in a schedule.
							</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="TimeRange" type="tsc:TimePeriod" minOccurs="0" maxOccurs="unbounded">
						<xs:annotation>
							<xs:documentation>
								Indicates the alternate time periods for the list of special days
								(overrides the regular schedule). For example, the regular schedule indicates
								that it is active from 8AM to 5PM on Mondays. However, this particular
								Monday is a special day, and the alternate time periods state that the
								schedule is active from 9 AM to 11 AM and 1 PM to 4 PM.
								If no time periods are defined, then no access is allowed.
								Is of type TimePeriod.
							</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Extension" type="tsc:SpecialDaysScheduleExtension" minOccurs="0"/>
				</xs:sequence>
				<xs:anyAttribute processContents="lax"/>
			</xs:complexType>
			<xs:complexType name="SpecialDaysScheduleExtension">
				<xs:sequence>
					<xs:any namespace="##any" processContents="lax" minOccurs="0" maxOccurs="unbounded"/>   <!-- first ONVIF then Vendor -->
				</xs:sequence>
			</xs:complexType>
			<!-- End of definition -->
			<!-- ScheduleState definitions-->
			<xs:complexType name="ScheduleState">
				<xs:annotation>
					<xs:documentation>
						The ScheduleState contains state information for a schedule.
					</xs:documentation>
				</xs:annotation>
				<xs:sequence>
					<xs:element name="Active" type="xs:boolean">
						<xs:annotation>
							<xs:documentation>
								Indicates that the current time is within the boundaries of the schedule
								or its special days schedules’ time periods. For example, if this
								schedule is being used for triggering automatic recording on a video source,
								the Active flag will be true when the schedule-based recording is supposed to record.
							</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="SpecialDay" type="xs:boolean" minOccurs="0">
						<xs:annotation>
							<xs:documentation>
								Indicates that the current time is within the boundaries of its special
								days schedules’ time periods. For example, if this schedule is being used
								for recording at a lower frame rate on a video source during special days,
								the SpecialDay flag will be true. If special days are not supported by the device,
								this field may be omitted and interpreted as false by the client.
							</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Extension" type="tsc:ScheduleStateExtension" minOccurs="0"/>
				</xs:sequence>
				<xs:anyAttribute processContents="lax"/>
			</xs:complexType>
			<xs:complexType name="ScheduleStateExtension">
				<xs:sequence>
					<xs:any namespace="##any" processContents="lax" minOccurs="0" maxOccurs="unbounded"/>   <!-- first ONVIF then Vendor -->
				</xs:sequence>
			</xs:complexType>
			<!-- End of definition -->
			<!--TimePeriod definitions-->
			<xs:complexType name="TimePeriod">
				<xs:annotation>
					<xs:documentation>
						A time period defines a start and end time. For full day access, the
						start time ="00:00:00" with no defined end time. For a time period with no
						end time, the schedule runs until midnight. The end time must always be greater
						than the start time, otherwise an InvalidArgVal error messages is generated by the device.
					</xs:documentation>
				</xs:annotation>
				<xs:sequence>
					<xs:element name="From" type="xs:time">
						<xs:annotation>
							<xs:documentation>
								Indicates the start time.
							</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Until" type="xs:time" minOccurs="0">
						<xs:annotation>
							<xs:documentation>
								Indicates the end time. Is optional, if omitted, the period ends at midnight.
								The end time is exclusive, meaning that that exact moment in time is not
								part of the period. To determine if a moment in time (t) is part of a time period,
								the formula StartTime &#8804; t &lt; EndTime is used.
							</xs:documentation>
						</xs:annotation>
					</xs:element>
					<xs:element name="Extension" type="tsc:TimePeriodExtension" minOccurs="0"/>
				</xs:sequence>
				<xs:anyAttribute processContents="lax"/>
			</xs:complexType>
			<xs:complexType name="TimePeriodExtension">
				<xs:sequence>
					<xs:any namespace="##any" processContents="lax" minOccurs="0" maxOccurs="unbounded"/>   <!-- first ONVIF then Vendor -->
				</xs:sequence>
			</xs:complexType>
			<!-- End of definition -->
			<!--SpecialDaysInfo definitions-->
			<xs:complexType name="SpecialDayGroupInfo">
				<xs:annotation>
					<xs:documentation>
						The SpecialDayGroupInfo structure contains the basic information about the special days list.
					</xs:documentation>
				</xs:annotation>
				<xs:complexContent>
					<xs:extension base="pt:DataEntity">
						<xs:sequence>
							<xs:element name="Name" type="pt:Name">
								<xs:annotation>
									<xs:documentation>
										User readable name. It shall be up to 64 characters.
									</xs:documentation>
								</xs:annotation>
							</xs:element>
							<xs:element name="Description" type="pt:Description" minOccurs="0">
								<xs:annotation>
									<xs:documentation>
										User readable description for the special days. It shall be up to 1024
										characters.
									</xs:documentation>
								</xs:annotation>
							</xs:element>
						</xs:sequence>
					</xs:extension>
				</xs:complexContent>
			</xs:complexType>
			<!-- End of definition -->
			<!--SpecialDays definitions-->
			<xs:complexType name="SpecialDayGroup">
				<xs:annotation>
					<xs:documentation>
						The special day group structure shall include all properties of the SpecialDayGroupInfo
						structure and also a set of special days. A special day group are days (or parts of days)
						that require the regular schedule to be overridden with an alternate schedule.
						For example holidays, half-days, working Sundays, etc.
					</xs:documentation>
				</xs:annotation>
				<xs:complexContent>
					<xs:extension base="tsc:SpecialDayGroupInfo">
						<xs:sequence>
							<xs:element name="Days" type="xs:string" minOccurs="0">
								<xs:annotation>
									<xs:documentation>
										An iCalendar structure that contains a group of special days.
										Is of type string (containing an iCalendar structure).
									</xs:documentation>
								</xs:annotation>
							</xs:element>
							<xs:element name="Extension" type="tsc:SpecialDayGroupExtension" minOccurs="0"/>
						</xs:sequence>
						<xs:anyAttribute processContents="lax"/>
					</xs:extension>
				</xs:complexContent>
			</xs:complexType>
			<xs:complexType name="SpecialDayGroupExtension">
				<xs:sequence>
					<xs:any namespace="##any" processContents="lax" minOccurs="0" maxOccurs="unbounded"/>   <!-- first ONVIF then Vendor -->
				</xs:sequence>
			</xs:complexType>
			<!-- End of definition -->
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
						<xs:element name="Capabilities" type="tsc:ServiceCapabilities">
							<xs:annotation>
								<xs:documentation>
									The capability response message contains the requested schedule service
									capabilities using a hierarchical XML capability structure.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetScheduleState">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>
									Token of schedule instance to get ScheduleState.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetScheduleStateResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="ScheduleState" type="tsc:ScheduleState">
							<xs:annotation>
								<xs:documentation>
									ScheduleState item.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetScheduleInfo">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken" minOccurs="1" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>
									Tokens of ScheduleInfo items to get.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetScheduleInfoResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="ScheduleInfo" type="tsc:ScheduleInfo" minOccurs="0" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>
									List of ScheduleInfo items.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetScheduleInfoList">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Limit" type="xs:int" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									Maximum number of entries to return. If not specified, less than one
									or higher than what the device supports, the number of items is
									determined by the device.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="StartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									Start returning entries from this start reference.
									If not specified, entries shall start from the beginning of the dataset.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetScheduleInfoListResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="NextStartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									StartReference to use in next call to get the following items.
									If absent, no more items to get.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="ScheduleInfo" type="tsc:ScheduleInfo" minOccurs="0" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>
									List of ScheduleInfo items.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSchedules">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken" minOccurs="1" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>Tokens of Schedule items to get</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSchedulesResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Schedule" type="tsc:Schedule" minOccurs="0" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of schedule items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetScheduleList">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Limit" type="xs:int" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									Maximum number of entries to return.
									If not specified, less than one or higher than what the device supports,
									the number of items is determined by the device.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="StartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									Start returning entries from this start reference.
									If not specified, entries shall start from the beginning of the dataset.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetScheduleListResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="NextStartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									StartReference to use in next call to get the following items.
									If absent, no more items to get.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="Schedule" type="tsc:Schedule" minOccurs="0" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of Schedule items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="CreateSchedule">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Schedule" type="tsc:Schedule" minOccurs="1">
							<xs:annotation>
								<xs:documentation>The Schedule to create</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="CreateScheduleResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>The token of created Schedule</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="SetSchedule">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Schedule" type="tsc:Schedule" minOccurs="1">
							<xs:annotation>
								<xs:documentation>The Schedule to modify/create</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="SetScheduleResponse">
				<xs:complexType>
					<xs:sequence>
					</xs:sequence>
				</xs:complexType>
			</xs:element>			
			<!--===============================-->
			<xs:element name="ModifySchedule">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Schedule" type="tsc:Schedule" minOccurs="1">
							<xs:annotation>
								<xs:documentation>The Schedule to modify/update</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="ModifyScheduleResponse">
				<xs:complexType>
					<xs:sequence>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="DeleteSchedule">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken" minOccurs="1">
							<xs:annotation>
								<xs:documentation>The token of the schedule to delete.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="DeleteScheduleResponse">
				<xs:complexType>
					<xs:sequence>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSpecialDayGroupInfo">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken" minOccurs="1" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>Tokens of SpecialDayGroupInfo items to get.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSpecialDayGroupInfoResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="SpecialDayGroupInfo" type="tsc:SpecialDayGroupInfo" minOccurs="0"
									maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of SpecialDayGroupInfo items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSpecialDayGroupInfoList">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Limit" type="xs:int" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									Maximum number of entries to return. If not specified, less than
									one or higher than what the device supports, the number
									of items is determined by the device.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="StartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									Start returning entries from this start reference.
									If not specified, entries shall start from the beginning of the dataset.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSpecialDayGroupInfoListResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="NextStartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									StartReference to use in next call to get the following items.
									If absent, no more items to get.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="SpecialDayGroupInfo" type="tsc:SpecialDayGroupInfo" minOccurs="0"
									maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of SpecialDayGroupInfo items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSpecialDayGroups">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken" minOccurs="1" maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>Tokens of the SpecialDayGroup items to get</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSpecialDayGroupsResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="SpecialDayGroup" type="tsc:SpecialDayGroup" minOccurs="0"
									maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of SpecialDayGroup items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSpecialDayGroupList">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Limit" type="xs:int" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									Maximum number of entries to return. If not specified, less than
									one or higher than what the device supports, the number of
									items is determined by the device.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="StartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>
									Start returning entries from this start reference.
									If not specified, entries shall start from the beginning of the dataset.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="GetSpecialDayGroupListResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="NextStartReference" type="xs:string" minOccurs="0">
							<xs:annotation>
								<xs:documentation>StartReference to use in next call to get the following items. If
									absent, no more items to get.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
						<xs:element name="SpecialDayGroup" type="tsc:SpecialDayGroup" minOccurs="0"
									maxOccurs="unbounded">
							<xs:annotation>
								<xs:documentation>List of SpecialDayGroup items.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="CreateSpecialDayGroup">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="SpecialDayGroup" type="tsc:SpecialDayGroup" minOccurs="1">
							<xs:annotation>
								<xs:documentation>
									The special day group to create.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="CreateSpecialDayGroupResponse">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken" minOccurs="1">
							<xs:annotation>
								<xs:documentation>The token of created special day group.</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="SetSpecialDayGroup">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="SpecialDayGroup" type="tsc:SpecialDayGroup" minOccurs="1">
							<xs:annotation>
								<xs:documentation>The SpecialDayGroup to modify/create</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="SetSpecialDayGroupResponse">
				<xs:complexType>
					<xs:sequence>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="ModifySpecialDayGroup">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="SpecialDayGroup" type="tsc:SpecialDayGroup" minOccurs="1">
							<xs:annotation>
								<xs:documentation>
									The special day group to modify/update.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="ModifySpecialDayGroupResponse">
				<xs:complexType>
					<xs:sequence>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="DeleteSpecialDayGroup">
				<xs:complexType>
					<xs:sequence>
						<xs:element name="Token" type="pt:ReferenceToken">
							<xs:annotation>
								<xs:documentation>
									The token of the special day group item to delete.
								</xs:documentation>
							</xs:annotation>
						</xs:element>
					</xs:sequence>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<xs:element name="DeleteSpecialDayGroupResponse">
				<xs:complexType>
					<xs:sequence/>
				</xs:complexType>
			</xs:element>
			<!--===============================-->
			<!-- #################-->
		</xs:schema>
	</wsdl:types>
	<!--Definition of 'GetServiceCapabilities' message-->
	<wsdl:message name="GetServiceCapabilitiesRequest">
		<wsdl:part name="parameters" element="tsc:GetServiceCapabilities"/>
	</wsdl:message>
	<wsdl:message name="GetServiceCapabilitiesResponse">
		<wsdl:part name="parameters" element="tsc:GetServiceCapabilitiesResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetScheduleState' message-->
	<wsdl:message name="GetScheduleStateRequest">
		<wsdl:part name="parameters" element="tsc:GetScheduleState"/>
	</wsdl:message>
	<wsdl:message name="GetScheduleStateResponse">
		<wsdl:part name="parameters" element="tsc:GetScheduleStateResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetScheduleInfo' message-->
	<wsdl:message name="GetScheduleInfoRequest">
		<wsdl:part name="parameters" element="tsc:GetScheduleInfo"/>
	</wsdl:message>
	<wsdl:message name="GetScheduleInfoResponse">
		<wsdl:part name="parameters" element="tsc:GetScheduleInfoResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetScheduleInfoList' message-->
	<wsdl:message name="GetScheduleInfoListRequest">
		<wsdl:part name="parameters" element="tsc:GetScheduleInfoList"/>
	</wsdl:message>
	<wsdl:message name="GetScheduleInfoListResponse">
		<wsdl:part name="parameters" element="tsc:GetScheduleInfoListResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetSchedules' message-->
	<wsdl:message name="GetSchedulesRequest">
		<wsdl:part name="parameters" element="tsc:GetSchedules"/>
	</wsdl:message>
	<wsdl:message name="GetSchedulesResponse">
		<wsdl:part name="parameters" element="tsc:GetSchedulesResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetScheduleList' message-->
	<wsdl:message name="GetScheduleListRequest">
		<wsdl:part name="parameters" element="tsc:GetScheduleList"/>
	</wsdl:message>
	<wsdl:message name="GetScheduleListResponse">
		<wsdl:part name="parameters" element="tsc:GetScheduleListResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'CreateSchedule' message-->
	<wsdl:message name="CreateScheduleRequest">
		<wsdl:part name="parameters" element="tsc:CreateSchedule"/>
	</wsdl:message>
	<wsdl:message name="CreateScheduleResponse">
		<wsdl:part name="parameters" element="tsc:CreateScheduleResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'SetSchedule' message-->
	<wsdl:message name="SetScheduleRequest">
		<wsdl:part name="parameters" element="tsc:SetSchedule"/>
	</wsdl:message>
	<wsdl:message name="SetScheduleResponse">
		<wsdl:part name="parameters" element="tsc:SetScheduleResponse"/>
	</wsdl:message>	
	<!--===============================-->
	<!--Definition of 'ModifySchedule' message-->
	<wsdl:message name="ModifyScheduleRequest">
		<wsdl:part name="parameters" element="tsc:ModifySchedule"/>
	</wsdl:message>
	<wsdl:message name="ModifyScheduleResponse">
		<wsdl:part name="parameters" element="tsc:ModifyScheduleResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'DeleteSchedule' message-->
	<wsdl:message name="DeleteScheduleRequest">
		<wsdl:part name="parameters" element="tsc:DeleteSchedule"/>
	</wsdl:message>
	<wsdl:message name="DeleteScheduleResponse">
		<wsdl:part name="parameters" element="tsc:DeleteScheduleResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetSpecialDayGroupInfo' message-->
	<wsdl:message name="GetSpecialDayGroupInfoRequest">
		<wsdl:part name="parameters" element="tsc:GetSpecialDayGroupInfo"/>
	</wsdl:message>
	<wsdl:message name="GetSpecialDayGroupInfoResponse">
		<wsdl:part name="parameters" element="tsc:GetSpecialDayGroupInfoResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetSpecialDayGroupInfoList' message-->
	<wsdl:message name="GetSpecialDayGroupInfoListRequest">
		<wsdl:part name="parameters" element="tsc:GetSpecialDayGroupInfoList"/>
	</wsdl:message>
	<wsdl:message name="GetSpecialDayGroupInfoListResponse">
		<wsdl:part name="parameters" element="tsc:GetSpecialDayGroupInfoListResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetSpecialDayGroups' message-->
	<wsdl:message name="GetSpecialDayGroupsRequest">
		<wsdl:part name="parameters" element="tsc:GetSpecialDayGroups"/>
	</wsdl:message>
	<wsdl:message name="GetSpecialDayGroupsResponse">
		<wsdl:part name="parameters" element="tsc:GetSpecialDayGroupsResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'GetSpecialDayGroupList' message-->
	<wsdl:message name="GetSpecialDayGroupListRequest">
		<wsdl:part name="parameters" element="tsc:GetSpecialDayGroupList"/>
	</wsdl:message>
	<wsdl:message name="GetSpecialDayGroupListResponse">
		<wsdl:part name="parameters" element="tsc:GetSpecialDayGroupListResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'CreateSpecialDayGroup' message-->
	<wsdl:message name="CreateSpecialDayGroupRequest">
		<wsdl:part name="parameters" element="tsc:CreateSpecialDayGroup"/>
	</wsdl:message>
	<wsdl:message name="CreateSpecialDayGroupResponse">
		<wsdl:part name="parameters" element="tsc:CreateSpecialDayGroupResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'SetSpecialDayGroup' message-->
	<wsdl:message name="SetSpecialDayGroupRequest">
		<wsdl:part name="parameters" element="tsc:SetSpecialDayGroup"/>
	</wsdl:message>
	<wsdl:message name="SetSpecialDayGroupResponse">
		<wsdl:part name="parameters" element="tsc:SetSpecialDayGroupResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'ModifySpecialDayGroup' message-->
	<wsdl:message name="ModifySpecialDayGroupRequest">
		<wsdl:part name="parameters" element="tsc:ModifySpecialDayGroup"/>
	</wsdl:message>
	<wsdl:message name="ModifySpecialDayGroupResponse">
		<wsdl:part name="parameters" element="tsc:ModifySpecialDayGroupResponse"/>
	</wsdl:message>
	<!--===============================-->
	<!--Definition of 'DeleteSpecialDayGroup' message-->
	<wsdl:message name="DeleteSpecialDayGroupRequest">
		<wsdl:part name="parameters" element="tsc:DeleteSpecialDayGroup"/>
	</wsdl:message>
	<wsdl:message name="DeleteSpecialDayGroupResponse">
		<wsdl:part name="parameters" element="tsc:DeleteSpecialDayGroupResponse"/>
	</wsdl:message>
	<!--===============================-->
	<wsdl:portType name="SchedulePort">
		<wsdl:operation name="GetServiceCapabilities">
			<wsdl:documentation>
				This operation returns the capabilities of the schedule service.
			</wsdl:documentation>
			<wsdl:input message="tsc:GetServiceCapabilitiesRequest"/>
			<wsdl:output message="tsc:GetServiceCapabilitiesResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetScheduleState">
			<wsdl:documentation>
				This operation requests the ScheduleState for the schedule instance specified by the given token.
			</wsdl:documentation>
			<wsdl:input message="tsc:GetScheduleStateRequest"/>
			<wsdl:output message="tsc:GetScheduleStateResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetScheduleInfo">
			<wsdl:documentation>
				This method returns a list of schedule info items, specified in the request.
				Only found schedules shall be returned, i.e., the returned numbers of elements can
				differ from the requested element.
				The device shall ignore tokens it cannot resolve and shall return an empty list if
				there are no items matching the specified tokens.
				If the number of requested items is greater than MaxLimit, a TooManyItems fault shall be returned.
			</wsdl:documentation>
			<wsdl:input message="tsc:GetScheduleInfoRequest"/>
			<wsdl:output message="tsc:GetScheduleInfoResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetScheduleInfoList">
			<wsdl:documentation>
				This operation requests a list of all of ScheduleInfo items provided by the device.
				A call to this method shall return a StartReference when not all data is returned
				and more data is available. The reference shall be valid for retrieving the next set of data.
				Please refer Access Control Service Specification for more details.
				The number of items returned shall not be greater the Limit parameter.
			</wsdl:documentation>
			<wsdl:input message="tsc:GetScheduleInfoListRequest"/>
			<wsdl:output message="tsc:GetScheduleInfoListResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetSchedules">
			<wsdl:documentation>
				This operation returns the specified schedule item matching the given tokens.
				The device shall ignore tokens it cannot resolve and shall return an empty list
				if there are no items matching the specified tokens.
				If the number of requested items is greater than MaxLimit, a TooManyItems fault shall be returned
			</wsdl:documentation>
			<wsdl:input message="tsc:GetSchedulesRequest"/>
			<wsdl:output message="tsc:GetSchedulesResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetScheduleList">
			<wsdl:documentation>
				This operation requests a list of all of Schedule items provided by the device.
				A call to this method shall return a StartReference when not all data is returned
				and more data is available. The reference shall be valid for retrieving the next set of data.
				Please refer Access Control Service Specification for more details.
				The number of items returned shall not be greater the Limit parameter.
			</wsdl:documentation>
			<wsdl:input message="tsc:GetScheduleListRequest"/>
			<wsdl:output message="tsc:GetScheduleListResponse"/>
		</wsdl:operation>
		<wsdl:operation name="CreateSchedule">
			<wsdl:documentation>
				This operation creates the specified schedule. The token field of the schedule structure
				shall be empty, the device shall allocate a token for the schedule. The allocated token
				shall be returned in the response. If the client sends any value in the token field,
				the device shall return InvalidArgVal as generic fault code.
			</wsdl:documentation>
			<wsdl:input message="tsc:CreateScheduleRequest"/>
			<wsdl:output message="tsc:CreateScheduleResponse"/>
		</wsdl:operation>
		<wsdl:operation name="SetSchedule">
			<wsdl:documentation>
				This operation modifies or creates the specified schedule.
			</wsdl:documentation>
			<wsdl:input message="tsc:SetScheduleRequest"/>
			<wsdl:output message="tsc:SetScheduleResponse"/>
		</wsdl:operation>
		<wsdl:operation name="ModifySchedule">
			<wsdl:documentation>
				This operation modifies or updates the specified schedule.
			</wsdl:documentation>
			<wsdl:input message="tsc:ModifyScheduleRequest"/>
			<wsdl:output message="tsc:ModifyScheduleResponse"/>
		</wsdl:operation>
		<wsdl:operation name="DeleteSchedule">
			<wsdl:documentation>
				This operation will delete the specified schedule.
				If it is associated with one or more entities some devices may not be able to delete the schedule,
				and consequently a ReferenceInUse fault shall be generated.
			</wsdl:documentation>
			<wsdl:input message="tsc:DeleteScheduleRequest"/>
			<wsdl:output message="tsc:DeleteScheduleResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetSpecialDayGroupInfo">
			<wsdl:documentation>
				This operation requests a list of SpecialDayGroupInfo items matching the given tokens.
				The device shall ignore tokens it cannot resolve and shall return an empty list if
				there are no items matching specified tokens. The device shall not return a fault in this case.
				If the number of requested items is greater than MaxLimit, a TooManyItems fault shall be returned.
			</wsdl:documentation>
			<wsdl:input message="tsc:GetSpecialDayGroupInfoRequest"/>
			<wsdl:output message="tsc:GetSpecialDayGroupInfoResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetSpecialDayGroupInfoList">
			<wsdl:documentation>
				This operation requests a list of all of SpecialDayGroupInfo items provided by the device.
				A call to this method shall return a StartReference when not all data is returned and
				more data is available. The reference shall be valid for retrieving the next set of data.
				The number of items returned shall not be greater than Limit parameter.
			</wsdl:documentation>
			<wsdl:input message="tsc:GetSpecialDayGroupInfoListRequest"/>
			<wsdl:output message="tsc:GetSpecialDayGroupInfoListResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetSpecialDayGroups">
			<wsdl:documentation>
				This operation returns the specified special day group item matching the given token.
			</wsdl:documentation>
			<wsdl:input message="tsc:GetSpecialDayGroupsRequest"/>
			<wsdl:output message="tsc:GetSpecialDayGroupsResponse"/>
		</wsdl:operation>
		<wsdl:operation name="GetSpecialDayGroupList">
			<wsdl:documentation>
				This operation requests a list of all of SpecialDayGroupList items provided by the device.
				A call to this method shall return a StartReference when not all data is returned and
				more data is available. The reference shall be valid for retrieving the next set of data.
				Please refer Access Control Service Specification for more details.
				The number of items returned shall not be greater the Limit parameter.
			</wsdl:documentation>
			<wsdl:input message="tsc:GetSpecialDayGroupListRequest"/>
			<wsdl:output message="tsc:GetSpecialDayGroupListResponse"/>
		</wsdl:operation>
		<wsdl:operation name="CreateSpecialDayGroup">
			<wsdl:documentation>
				This operation creates the specified special day group. The token field of the
				SpecialDayGroup structure shall be empty, the device shall allocate a token for the
				special day group. The allocated token shall be returned in the response.
				If there is any value in the token field, the device shall return InvalidArgVal as generic fault code.
			</wsdl:documentation>
			<wsdl:input message="tsc:CreateSpecialDayGroupRequest"/>
			<wsdl:output message="tsc:CreateSpecialDayGroupResponse"/>
		</wsdl:operation>
		<wsdl:operation name="SetSpecialDayGroup">
			<wsdl:documentation>
				This operation modifies or creates the specified special day group.
			</wsdl:documentation>
			<wsdl:input message="tsc:SetSpecialDayGroupRequest"/>
			<wsdl:output message="tsc:SetSpecialDayGroupResponse"/>
		</wsdl:operation>
		<wsdl:operation name="ModifySpecialDayGroup">
			<wsdl:documentation>
				This operation updates the specified special day group.
			</wsdl:documentation>
			<wsdl:input message="tsc:ModifySpecialDayGroupRequest"/>
			<wsdl:output message="tsc:ModifySpecialDayGroupResponse"/>
		</wsdl:operation>
		<wsdl:operation name="DeleteSpecialDayGroup">
			<wsdl:documentation>
				This method deletes the specified special day group.
				If it is associated with one or more schedules some devices may not be able to delete
				the special day group, and consequently a ReferenceInUse fault must be generated.
			</wsdl:documentation>
			<wsdl:input message="tsc:DeleteSpecialDayGroupRequest"/>
			<wsdl:output message="tsc:DeleteSpecialDayGroupResponse"/>
		</wsdl:operation>
	</wsdl:portType>
	<!--Protocol & data format for the operations and messages for the port type 'SchedulePort'-->
	<wsdl:binding name="ScheduleBinding" type="tsc:SchedulePort">
		<soap:binding style="document" transport="http://schemas.xmlsoap.org/soap/http"/>
		<wsdl:operation name="GetServiceCapabilities">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetServiceCapabilities"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetScheduleState">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetScheduleState"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetScheduleInfo">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetScheduleInfo"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetScheduleInfoList">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetScheduleInfoList"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetSchedules">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetSchedules"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetScheduleList">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetScheduleList"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="CreateSchedule">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/CreateSchedule"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="SetSchedule">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/SetSchedule"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="ModifySchedule">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/ModifySchedule"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="DeleteSchedule">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/DeleteSchedule"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetSpecialDayGroupInfo">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetSpecialDayGroupInfo"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetSpecialDayGroupInfoList">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetSpecialDayGroupInfoList"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetSpecialDayGroups">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetSpecialDayGroups"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="GetSpecialDayGroupList">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/GetSpecialDayGroupList"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="CreateSpecialDayGroup">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/CreateSpecialDayGroup"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="SetSpecialDayGroup">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/SetSpecialDayGroup"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="ModifySpecialDayGroup">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/ModifySpecialDayGroup"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
		<wsdl:operation name="DeleteSpecialDayGroup">
			<soap:operation soapAction="http://www.onvif.org/ver10/schedule/wsdl/DeleteSpecialDayGroup"/>
			<wsdl:input>
				<soap:body use="literal"/>
			</wsdl:input>
			<wsdl:output>
				<soap:body use="literal"/>
			</wsdl:output>
		</wsdl:operation>
	</wsdl:binding>
</wsdl:definitions>
