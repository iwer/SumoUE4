// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SumoVehicleType.h"
#include "SumoLane.h"
#include "SumoRoadType.h"
#include "SumoConnection.h"

#include "SumoEdge.generated.h"

UENUM(BlueprintType)
enum ESumoEdgeFunction {
    SEF_Normal UMETA(ToolTip = "The edge is a plain part of a road network, like a highway or a normal street which connects two roads"),
    SEF_Internal UMETA(ToolTip = "The edge is a macroscopic connector - not a part of the real world road network"),
    SEF_Connector UMETA(ToolTip = "The edge is a part of an intersection (is located within the intersection)"),
    SEF_Crossing,
    SEF_WalkingArea
};

UENUM(BlueprintType)
enum ESumoEdgeSpreadType {
    SEST_Right,
    SEST_RoadCenter,
    SEST_Center
};

USTRUCT(Blueprintable, BlueprintType)
struct FSumoEdge {
    GENERATED_BODY()

    /**
     * The id of the edge
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ID;

    /**
     * road name (optional)
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Name;

    /**
     * type of this road
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FSumoRoadType Type;

    /**
     * The id of the junction it starts at
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString FromJunction;

    /**
     * The id of the junction it ends at
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ToJunction;

    /**
     * Indicates how important the road is (optional)
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Priority;

    /**
     * An abstract edge purpose (optional with default "normal")
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TEnumAsByte<ESumoEdgeFunction> Function;

    /**
     * Lanes this edge consists of
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FSumoLane> Lanes;

    /**
     * Stop offset
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FSumoStopOffset StopOffset;

    FSumoEdge() {
        ID = TEXT("");
        Name = TEXT("");
        //Type = TEXT("");
        FromJunction = TEXT("");
        ToJunction = TEXT("");
        Priority = 0;
        Function = ESumoEdgeFunction::SEF_Normal;
        Lanes.Empty();
    }
};
