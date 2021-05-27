// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "SumoConnection.generated.h"

UENUM(BlueprintType)
enum ESumoConnectionDirection {
    SCD_Straight,
    SCD_Turn,
    SCD_Left,
    SCD_Right,
    SCD_PartiallyLeft,
    SCD_PartiallyRight,
    SCD_Invalid
};

UENUM(BlueprintType)
enum ESumoConnectionState {
    SCS_DeadEnd,
    SCS_Equal,
    SCS_MinorLink,
    SCS_MajorLink,
    SCS_ControllerOff,
    SCS_YellowFlashing,
    SCS_YellowMinorLink,
    SCS_YellowMajorLink,
    SCS_Red,
    SCS_GreenMinor,
    SCS_GreenMajor
};


USTRUCT(Blueprintable, BlueprintType)
struct FSumoConnection {
    GENERATED_BODY()

    /**
     * The ID of the incoming edge at which the connection begins
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString FromEdgeID;

    /**
     * The ID of the outgoing edge at which the connection ends
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ToEdgeID;

    /**
     * The lane of the incoming edge at which the connection begins
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int FromLaneIndex;

    /**
     * The lane of the outgoing edge at which the connection ends
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int ToLaneIndex;

    /**
     * The id of the lane to use to pass this connection across the junction
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ViaLaneID;

    /**
     * The id of the traffic light that controls this connection;
     * the attribute is missing if the connection is not controlled by a traffic light
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString TrafficLightID;

    /**
     * The index of the signal responsible for the connection within the traffic light;
     * the attribute is missing if the connection is not controlled by a traffic light
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int LinkIndex;

    /**
     * The direction of the connection
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TEnumAsByte<ESumoConnectionDirection> Direction;

    /**
     * The state of the connection
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TEnumAsByte<ESumoConnectionState> State;

    FSumoConnection() {
        FromEdgeID = TEXT("");
        ToEdgeID = TEXT("");
        FromLaneIndex = 0;
        ToLaneIndex = 0;
        ViaLaneID = TEXT("");
        TrafficLightID = TEXT("");
        LinkIndex = 0;
        Direction = ESumoConnectionDirection::SCD_Invalid;
        State = ESumoConnectionState::SCS_Equal;
    }
};
