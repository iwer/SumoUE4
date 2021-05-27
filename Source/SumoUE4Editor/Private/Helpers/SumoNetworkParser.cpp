// Copyright (c) Iwer Petersen. All rights reserved.

#include "SumoNetworkParser.h"
#include "GeoCoordinate.h"
#include "GeoReferenceHelper.h"
#include "GDALHelpers.h"

USumoNetworkParser::USumoNetworkParser()
    : Target(nullptr)
    , State(ParsingState::Root)
    , CurrentJunction(nullptr)
    , CurrentEdge(nullptr)
{}
USumoNetworkParser::~USumoNetworkParser(){}

bool USumoNetworkParser::ParseFile(const FString &filename, USumoNetworkAsset * asset) {
    FText outError;
    int32 outErrorNum;
    FString Text = "";
    Target = asset;
    bool success = FFastXml::ParseXmlFile(this,
                                        *filename,
                                        (TCHAR*)*Text,
                                        nullptr,
                                        false,
                                        false,
                                        outError,
                                        outErrorNum);
    return success;
}

//IFastXMLCallback
bool USumoNetworkParser::ProcessXmlDeclaration(const TCHAR* ElementData, int32 XmlFileLineNumber) {
    return true;
}

bool USumoNetworkParser::ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber) {
    if(!FCString::Stricmp(ElementName, TEXT("location"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Parsing location..."));
        State = ParsingState::Location;
    } else if(!FCString::Stricmp(ElementName, TEXT("type"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Parsing roadtype..."));
        CurrentRoadType = new FSumoRoadType();
        State = ParsingState::Type;
    } else if(!FCString::Stricmp(ElementName, TEXT("junction"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Parsing junction..."));
        CurrentJunction = new FSumoJunction();
        State = ParsingState::Junction;
    } else if(!FCString::Stricmp(ElementName, TEXT("request"))) {
        CurrentRequest = new FSumoRequest();
        State = ParsingState::Request;
    } else if(!FCString::Stricmp(ElementName, TEXT("edge"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Parsing edge..."));
        CurrentEdge = new FSumoEdge();
        State = ParsingState::Edge;
    } else if(!FCString::Stricmp(ElementName, TEXT("lane"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Parsing lane..."));
        CurrentLane = new FSumoLane();
        State = ParsingState::Lane;
    } else if(!FCString::Stricmp(ElementName, TEXT("stopOffset"))) {
        CurrentStopOffset = new FSumoStopOffset();
        if(State == ParsingState::Edge) {
            State = ParsingState::EdgeStopOffset;
        } else if(State == ParsingState::Lane) {
            State = ParsingState::LaneStopOffset;
        }
    } else if(!FCString::Stricmp(ElementName, TEXT("tlLogic"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Parsing tlLogic..."));
        CurrentTLProgram = new FSumoTrafficLightProgram();
        State = ParsingState::TLLogic;
    } else if(!FCString::Stricmp(ElementName, TEXT("phase"))) {
        CurrentTLPhase = new FSumoTrafficLightPhase();
        State = ParsingState::TLPhase;
    } else if(!FCString::Stricmp(ElementName, TEXT("connection"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Parsing connection..."));
        CurrentConnection = new FSumoConnection();
        State = ParsingState::Connection;
    }
    return true;
}

bool USumoNetworkParser::ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue) {
    FString val(AttributeValue);

    if(State == ParsingState::Location) {
        if(!FCString::Stricmp(AttributeName, TEXT("netOffset"))) {
            FString xOffStr, yOffStr;
            val.Split(TEXT(","), &xOffStr, &yOffStr);
            UTMOffsetX = FPlatformString::Atod(*xOffStr);
            UTMOffsetY = FPlatformString::Atod(*yOffStr);
            // UE_LOG(LogTemp, Warning, TEXT("USumoNetworkParser: Offset: %f, %f"), UTMOffsetX, UTMOffsetY)
        } else if(!FCString::Stricmp(AttributeName, TEXT("projParameter"))) {
            OGRSpatialReference crs;
            if(crs.importFromProj4(TCHAR_TO_ANSI(*val)) != OGRERR_NONE) {
                UE_LOG(LogTemp, Error, TEXT("USumoNetworkParser: Failed to import CRS!"));
                return false;
            }

            if(crs.IsProjected()) {
                UTMZone = crs.GetUTMZone(&bNorthernHemisphere);
            } else {
                UE_LOG(LogTemp, Error, TEXT("USumoNetworkParser: Unexpected GeoReference!"));
                return false;
            }

        }
    } else if(State == ParsingState::Type) {
        if(!FCString::Stricmp(AttributeName, TEXT("id"))) {
            CurrentRoadType->ID = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("priority"))) {
            CurrentRoadType->Priority = FPlatformString::Atoi(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("numLanes"))) {
            CurrentRoadType->NumLanes = FPlatformString::Atoi(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("speed"))) {
            CurrentRoadType->Speed = FPlatformString::Atof(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("allow"))) {
            TArray<FString> types;
            val.ParseIntoArray(types, TEXT(" "), true);
            for(auto &type : types) {
                CurrentRoadType->AllowedVehicleTypes.Add(ToVehicleTypeEnum(type));
            }
        } else if(!FCString::Stricmp(AttributeName, TEXT("disallow"))) {
            TArray<FString> types;
            val.ParseIntoArray(types, TEXT(" "), true);
            for(auto &type : types) {
                CurrentRoadType->DisallowedVehicleTypes.Add(ToVehicleTypeEnum(type));
            }
        } else if(!FCString::Stricmp(AttributeName, TEXT("oneway"))) {
            CurrentRoadType->bOneWay = (FPlatformString::Atoi(AttributeValue) != 0);
        } else if(!FCString::Stricmp(AttributeName, TEXT("width"))) {
            CurrentRoadType->Width = FPlatformString::Atof(AttributeValue);
        }
    } else if(State == ParsingState::Junction) {
        if(!FCString::Stricmp(AttributeName, TEXT("id"))) {
            CurrentJunction->ID = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("type"))) {
            if(!FCString::Stricmp(AttributeValue, TEXT("unknown"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_Unknown;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("traffic_light"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_TrafficLight;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("traffic_light_unregulated"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_TrafficLightUnregulated;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("traffic_light_right_on_red"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_TrafficLightRightOnRed;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("rail_signal"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_RailSignal;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("rail_crossing"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_RailCrossing;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("priority"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_Priority;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("priority_stop"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_PriorityStop;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("allway_stop"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_AllwayStop;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("zipper"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_Zipper;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("district"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_District;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("unregulated"))) {
                CurrentJunction->Type = ESumoJunctionType::SJT_Unregulated;
            }
        } else if(!FCString::Stricmp(AttributeName, TEXT("x"))) {
            UTMJunctionLocationX = FPlatformString::Atof(AttributeValue) - UTMOffsetX;
        } else if(!FCString::Stricmp(AttributeName, TEXT("y"))) {
            UTMJunctionLocationY = FPlatformString::Atof(AttributeValue) - UTMOffsetY;
        } else if(!FCString::Stricmp(AttributeName, TEXT("incLanes"))) {
            val.ParseIntoArray(CurrentJunction->IncomingLaneIDs, TEXT(" "), true);
        } else if(!FCString::Stricmp(AttributeName, TEXT("intLanes"))) {
            val.ParseIntoArray(CurrentJunction->InternalLaneIDs, TEXT(" "), true);
        } else if(!FCString::Stricmp(AttributeName, TEXT("shape"))) {
            CurrentJunction->Shape.Append(ProcessShape(val));
        }
    } else if(State == ParsingState::Request) {
        if(!FCString::Stricmp(AttributeName, TEXT("index"))) {
            CurrentRequest->Index = FPlatformString::Atoi(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("response"))) {
            CurrentRequest->Response = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("foes"))) {
            CurrentRequest->Foes = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("cont"))) {
            CurrentRequest->bContinue = FPlatformString::Atoi(AttributeValue) == 0 ? false : true;
        }
    } else if(State == ParsingState::Edge) {
        if(!FCString::Stricmp(AttributeName, TEXT("id"))) {
            CurrentEdge->ID = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("from"))) {
            CurrentEdge->FromJunction = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("to"))) {
            CurrentEdge->ToJunction = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("priority"))) {
            CurrentEdge->Priority = FPlatformString::Atoi(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("name"))) {
            CurrentEdge->Name = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("type"))) {
            CurrentEdge->Type = Target->RoadTypes[val];
        } else if(!FCString::Stricmp(AttributeName, TEXT("function"))) {
            if(!FCString::Stricmp(AttributeValue, TEXT("normal"))) {
            } else if(!FCString::Stricmp(AttributeValue, TEXT("connector"))) {
                CurrentEdge->Function = ESumoEdgeFunction::SEF_Connector;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("internal"))) {
                CurrentEdge->Function = ESumoEdgeFunction::SEF_Internal;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("crossing"))) {
                CurrentEdge->Function = ESumoEdgeFunction::SEF_Crossing;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("walkingarea"))) {
                CurrentEdge->Function = ESumoEdgeFunction::SEF_WalkingArea;
            }
        }
    } else if(State == ParsingState::Lane) {
        if(!FCString::Stricmp(AttributeName, TEXT("id"))) {
            CurrentLane->ID = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("index"))) {
            CurrentLane->Index = FPlatformString::Atoi(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("speed"))) {
            CurrentLane->Speed = FPlatformString::Atof(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("length"))) {
            CurrentLane->Length = FPlatformString::Atof(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("shape"))) {
            CurrentLane->Shape.Append(ProcessShape(val));
        }
    } else if(State == ParsingState::EdgeStopOffset || State == ParsingState::LaneStopOffset)  {
        if(!FCString::Stricmp(AttributeName, TEXT("value"))) {
            CurrentStopOffset->Value = FPlatformString::Atof(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("vClasses"))) {
            TArray<FString> classStr;
            val.ParseIntoArray(classStr, TEXT(" "), true);
            for(auto &vehicleclass : classStr) {
                CurrentStopOffset->VehicleClasses.Add(ToVehicleTypeEnum(vehicleclass));
            }
        } else if(!FCString::Stricmp(AttributeName, TEXT("exceptions"))) {
            TArray<FString> classStr;
            val.ParseIntoArray(classStr, TEXT(" "), true);
            for(auto &vehicleclass : classStr) {
                CurrentStopOffset->Exceptions.Add(ToVehicleTypeEnum(vehicleclass));
            }
        }

    } else if(State == ParsingState::TLLogic) {
        if(!FCString::Stricmp(AttributeName, TEXT("id"))) {
            CurrentTLProgram->ID = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("type"))) {
            if(!FCString::Stricmp(AttributeValue, TEXT("static"))) {
                CurrentTLProgram->Type = ESumoTrafficLightProgramType::STLPT_Static;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("actuated"))) {
                CurrentTLProgram->Type = ESumoTrafficLightProgramType::STLPT_Actuated;
            } else if(!FCString::Stricmp(AttributeValue, TEXT("delay_based"))) {
                CurrentTLProgram->Type = ESumoTrafficLightProgramType::STLPT_DelayBased;
            }
        } else if(!FCString::Stricmp(AttributeName, TEXT("programID"))) {
            CurrentTLProgram->ProgramID = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("offset"))) {
            CurrentTLProgram->TimeOffset = FPlatformString::Atoi(AttributeValue);
        }
    } else if(State == ParsingState::TLPhase) {
        if(!FCString::Stricmp(AttributeName, TEXT("duration"))) {
            CurrentTLPhase->Duration = FPlatformString::Atoi(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("state"))) {
            for (int32 i = 0; i < val.Len(); i++)
            {
                FString currChar = val.Mid(i, 1);
                if (currChar.Equals(TEXT("r"))) {
                    CurrentTLPhase->States.Add(ESumoTrafficLightSignalState::STLSS_Red);
                } else if (currChar.Equals(TEXT("y"))) {
                    CurrentTLPhase->States.Add(ESumoTrafficLightSignalState::STLSS_Yellow);
                } else if (currChar.Equals(TEXT("g"))) {
                    CurrentTLPhase->States.Add(ESumoTrafficLightSignalState::STLSS_Green);
                } else if (currChar.Equals(TEXT("G"))) {
                    CurrentTLPhase->States.Add(ESumoTrafficLightSignalState::STLSS_GreenPriority);
                } else if (currChar.Equals(TEXT("s"))) {
                    CurrentTLPhase->States.Add(ESumoTrafficLightSignalState::STLSS_RightGreen);
                } else if (currChar.Equals(TEXT("u"))) {
                    CurrentTLPhase->States.Add(ESumoTrafficLightSignalState::STLSS_RedYellow);
                } else if (currChar.Equals(TEXT("o"))) {
                    CurrentTLPhase->States.Add(ESumoTrafficLightSignalState::STLSS_OffBlink);
                } else if (currChar.Equals(TEXT("O"))) {
                    CurrentTLPhase->States.Add(ESumoTrafficLightSignalState::STLSS_Off);
                }

            }
        } else if(!FCString::Stricmp(AttributeName, TEXT("minDur"))) {
            CurrentTLPhase->MinDuration = FPlatformString::Atoi(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("maxDur"))) {
            CurrentTLPhase->MaxDuration = FPlatformString::Atoi(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("name"))) {
            CurrentTLPhase->Name = val;
        } else if(!FCString::Stricmp(AttributeName, TEXT("next"))) {
            TArray<FString> indexStr;
            val.ParseIntoArray(indexStr, TEXT(" "), true);
            for(auto &ind : indexStr) {
                CurrentTLPhase->Next.Add(FPlatformString::Atoi(TCHAR_TO_ANSI(*ind)));
            }
        }
    } else if(State == ParsingState::Connection) {
        if(!FCString::Stricmp(AttributeName, TEXT("from"))) {
            CurrentTLPhase->MinDuration = FPlatformString::Atoi(AttributeValue);
        } else if(!FCString::Stricmp(AttributeName, TEXT("to"))) {
        } else if(!FCString::Stricmp(AttributeName, TEXT("fromLane"))) {
        } else if(!FCString::Stricmp(AttributeName, TEXT("toLane"))) {
        } else if(!FCString::Stricmp(AttributeName, TEXT("via"))) {
        } else if(!FCString::Stricmp(AttributeName, TEXT("tl"))) {
        } else if(!FCString::Stricmp(AttributeName, TEXT("linkIndex"))) {
        } else if(!FCString::Stricmp(AttributeName, TEXT("dir"))) {
        } else if(!FCString::Stricmp(AttributeName, TEXT("state"))) {
        }
    }
    return true;
}

bool USumoNetworkParser::ProcessClose(const TCHAR* Element) {
    if(!FCString::Stricmp(Element, TEXT("location"))) {
        State = ParsingState::Root;
    } else if(!FCString::Stricmp(Element, TEXT("type"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Adding road type: %s"), *CurrentRoadType->ID);
        Target->RoadTypes.Add(CurrentRoadType->ID, *CurrentRoadType);
        delete CurrentRoadType;
        State = ParsingState::Root;
    } else if(!FCString::Stricmp(Element, TEXT("junction"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Adding junction: %s at UTM:%f, %f (Offset: %f, %f)"), *CurrentJunction->ID,
        //                                                                     UTMJunctionLocationX,
        //                                                                     UTMJunctionLocationY,
        //                                                                     UTMOffsetX,
        //                                                                     UTMOffsetY);
        CurrentJunction->Location = UGeoCoordinate(UTMJunctionLocationX,
                                                   UTMJunctionLocationY,
                                                   FGeoReferenceHelper::GetEPSGForUTM(UTMZone, bNorthernHemisphere != 0))
                                           .ToFVector2DInEPSG(UGeoCoordinate::EPSG_WGS84);
        Target->Junctions.Add(CurrentJunction->ID, *CurrentJunction);
        delete CurrentJunction;
        State = ParsingState::Root;
    } else if(!FCString::Stricmp(Element, TEXT("request"))) {
        CurrentJunction->Requests.Add(*CurrentRequest);
        delete CurrentRequest;
        State = ParsingState::Junction;
    } else if(!FCString::Stricmp(Element, TEXT("edge"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Adding edge: %s"), *CurrentEdge->ID);
        Target->Edges.Add(CurrentEdge->ID, *CurrentEdge);
        delete CurrentEdge;
        State = ParsingState::Root;
    } else if(!FCString::Stricmp(Element, TEXT("stopOffset")) && State == ParsingState::EdgeStopOffset) {
        CurrentEdge->StopOffset = *CurrentStopOffset;
        delete CurrentStopOffset;
        State = ParsingState::Edge;
    } else if(!FCString::Stricmp(Element, TEXT("stopOffset")) && State == ParsingState::LaneStopOffset) {
        CurrentLane->StopOffset = *CurrentStopOffset;
        delete CurrentStopOffset;
        State = ParsingState::Lane;
    } else if(!FCString::Stricmp(Element, TEXT("lane"))) {
        // UE_LOG(LogTemp,Warning,TEXT("Adding lane: %s"), *CurrentLane->ID);
        CurrentEdge->Lanes.Add(*CurrentLane);
        delete CurrentLane;
        State = ParsingState::Edge;
    } else if(!FCString::Stricmp(Element, TEXT("tlLogic"))) {
        Target->TrafficLightPrograms.Add(CurrentTLProgram->ID, *CurrentTLProgram);
        delete CurrentTLProgram;
        State = ParsingState::Root;
    } else if(!FCString::Stricmp(Element, TEXT("phase"))) {
        CurrentTLProgram->Phases.Add(*CurrentTLPhase);
        delete CurrentTLPhase;
        State = ParsingState::TLLogic;
    } else if(!FCString::Stricmp(Element, TEXT("connection"))) {
        Target->Connections.Add(*CurrentConnection);
        delete CurrentConnection;
        State = ParsingState::Root;
    }

    return true;
}

bool USumoNetworkParser::ProcessComment(const TCHAR* Comment) {
    return true;
}

TEnumAsByte<ESumoVehicleType> USumoNetworkParser::ToVehicleTypeEnum(FString &typestring)
{
    if(!FCString::Stricmp(*typestring, TEXT("passenger"))) {
        return ESumoVehicleType::SVT_Passenger;
    } else if(!FCString::Stricmp(*typestring, TEXT("private"))) {
        return ESumoVehicleType::SVT_Private;
    } else if(!FCString::Stricmp(*typestring, TEXT("taxi"))) {
        return ESumoVehicleType::SVT_Taxi;
    } else if(!FCString::Stricmp(*typestring, TEXT("bus"))) {
        return ESumoVehicleType::SVT_Bus;
    } else if(!FCString::Stricmp(*typestring, TEXT("coach"))) {
        return ESumoVehicleType::SVT_Coach;
    } else if(!FCString::Stricmp(*typestring, TEXT("delivery"))) {
        return ESumoVehicleType::SVT_Delivery;
    } else if(!FCString::Stricmp(*typestring, TEXT("truck"))) {
        return ESumoVehicleType::SVT_Truck;
    } else if(!FCString::Stricmp(*typestring, TEXT("trailer"))) {
        return ESumoVehicleType::SVT_Trailer;
    } else if(!FCString::Stricmp(*typestring, TEXT("emergency"))) {
        return ESumoVehicleType::SVT_Emergency;
    } else if(!FCString::Stricmp(*typestring, TEXT("motorcycle"))) {
        return ESumoVehicleType::SVT_Motorcycle;
    } else if(!FCString::Stricmp(*typestring, TEXT("moped"))) {
        return ESumoVehicleType::SVT_Moped;
    } else if(!FCString::Stricmp(*typestring, TEXT("bicycle"))) {
        return ESumoVehicleType::SVT_Bicycle;
    } else if(!FCString::Stricmp(*typestring, TEXT("pedestrian"))) {
        return ESumoVehicleType::SVT_Pedestrian;
    } else if(!FCString::Stricmp(*typestring, TEXT("tram"))) {
        return ESumoVehicleType::SVT_Tram;
    } else if(!FCString::Stricmp(*typestring, TEXT("rail_electric"))) {
        return ESumoVehicleType::SVT_RailElectric;
    } else if(!FCString::Stricmp(*typestring, TEXT("rail_fast"))) {
        return ESumoVehicleType::SVT_RailFast;
    } else if(!FCString::Stricmp(*typestring, TEXT("rail_urban"))) {
        return ESumoVehicleType::SVT_RailUrban;
    } else if(!FCString::Stricmp(*typestring, TEXT("rail"))) {
        return ESumoVehicleType::SVT_Rail;
    } else if(!FCString::Stricmp(*typestring, TEXT("evehicle"))) {
        return ESumoVehicleType::SVT_EVehicle;
    } else if(!FCString::Stricmp(*typestring, TEXT("army"))) {
        return ESumoVehicleType::SVT_Army;
    } else if(!FCString::Stricmp(*typestring, TEXT("ship"))) {
        return ESumoVehicleType::SVT_Ship;
    } else if(!FCString::Stricmp(*typestring, TEXT("authority"))) {
        return ESumoVehicleType::SVT_Authority;
    } else if(!FCString::Stricmp(*typestring, TEXT("vip"))) {
        return ESumoVehicleType::SVT_Vip;
    } else if(!FCString::Stricmp(*typestring, TEXT("hov"))) {
        return ESumoVehicleType::SVT_Hov;
    } else if(!FCString::Stricmp(*typestring, TEXT("custom1"))) {
        return ESumoVehicleType::SVT_Custom1;
    } else if(!FCString::Stricmp(*typestring, TEXT("custom2"))) {
        return ESumoVehicleType::SVT_Custom2;
    }
    return ESumoVehicleType::SVT_Passenger;
}

TArray<FVector> USumoNetworkParser::ProcessShape(FString &ShapeCoordinates)
{
    TArray<FString> shapeCoords;
    TArray<FVector> vertices;
    ShapeCoordinates.ParseIntoArray(shapeCoords, TEXT(" "), true);
    for(FString &coord : shapeCoords) {
        TArray<FString> components;
        coord.ParseIntoArray(components, TEXT(","), true);
        double x = FPlatformString::Atod(*components[0]) - UTMOffsetX;
        double y = FPlatformString::Atod(*components[1]) - UTMOffsetY;
        FVector2D geo = UGeoCoordinate(x, y, FGeoReferenceHelper::GetEPSGForUTM(UTMZone,
                                       bNorthernHemisphere != 0)).ToFVector2DInEPSG(UGeoCoordinate::EPSG_WGS84);
        double z = components.Num() > 2 ? FPlatformString::Atod(*components[2]) : 0.0;
        vertices.Add(FVector(geo, z));
        // UE_LOG(LogTemp, Warning, TEXT("ShapeCoord: UTM(%d): %.4f,%.4f WGS+Height: %s + %.4f"), UTMZone, x, y, *geo.ToString(), z);
    }
    return vertices;
}
