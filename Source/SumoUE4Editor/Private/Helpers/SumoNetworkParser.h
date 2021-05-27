// Copyright (c) Iwer Petersen. All rights reserved.
#pragma once


#include "Runtime/XmlParser/Public/FastXml.h"
#include "Runtime/XmlParser/Public/XmlParser.h"
#include "SumoNetworkAsset.h"

class USumoNetworkParser : public IFastXmlCallback
{
public:
    USumoNetworkParser();
    virtual ~USumoNetworkParser();

    bool ParseFile(const FString &filename, USumoNetworkAsset * asset);

protected:
    //IFastXMLCallback
	bool ProcessXmlDeclaration(const TCHAR* ElementData, int32 XmlFileLineNumber);
	bool ProcessElement(const TCHAR* ElementName, const TCHAR* ElementData, int32 XmlFileLineNumber);
	bool ProcessAttribute(const TCHAR* AttributeName, const TCHAR* AttributeValue);
	bool ProcessClose(const TCHAR* Element);
	bool ProcessComment(const TCHAR* Comment);

private:
    enum ParsingState {
        Root,
        Location,
        Type,
        Junction,
        Request,
        Edge,
        Lane,
        EdgeStopOffset,
        LaneStopOffset,
        TLLogic,
        TLPhase,
        Connection
    };

    USumoNetworkAsset * Target;
    ParsingState State;
    FSumoJunction * CurrentJunction;
    FSumoRequest * CurrentRequest;
    FSumoEdge * CurrentEdge;
    FSumoLane * CurrentLane;
    FSumoRoadType * CurrentRoadType;
    FSumoStopOffset * CurrentStopOffset;
    FSumoTrafficLightProgram * CurrentTLProgram;
    FSumoTrafficLightPhase * CurrentTLPhase;
    FSumoConnection * CurrentConnection;

    int UTMZone;
    int bNorthernHemisphere;

    double UTMOffsetX;
    double UTMOffsetY;

    double UTMJunctionLocationX;
    double UTMJunctionLocationY;

    TEnumAsByte<ESumoVehicleType> ToVehicleTypeEnum(FString &typestring);
    TArray<FVector> ProcessShape(FString & ShapeCoordinates);
};
