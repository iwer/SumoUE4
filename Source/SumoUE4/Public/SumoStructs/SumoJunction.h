// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SumoRequest.h"

#include "SumoJunction.generated.h"

UENUM(BlueprintType)
enum ESumoJunctionType {
    SJT_Unknown,
    SJT_TrafficLight,
    SJT_TrafficLightUnregulated,
    SJT_TrafficLightRightOnRed,
    SJT_RailSignal,
    SJT_RailCrossing,
    SJT_Priority,
    SJT_PriorityStop,
    SJT_RightBeforeLeft,
    SJT_AllwayStop,
    SJT_Zipper,
    SJT_District,
    SJT_Unregulated
};


USTRUCT(Blueprintable, BlueprintType)
struct FSumoJunction {
    GENERATED_BODY()

    /**
     * The id of the junction; please note, that a traffic light definition
     * must use the same ID when controlling this intersection.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ID;//

    /**
     * Location of this junction in WGS84 Long,Lat
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FVector2D Location;

    /**
     * Junction type
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TEnumAsByte<ESumoJunctionType> Type;

    /**
     * A polygon describing the road boundaries of the intersection in WGS84 Long,Lat
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FVector> Shape;

    /**
     * The ids of the lanes that end at the intersection; sorted by direction, clockwise, with direction up = 0
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FString> IncomingLaneIDs;

    /**
     * The IDs of the lanes within the intersection
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FString> InternalLaneIDs;

    /**
     * Requests
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FSumoRequest> Requests;

    FSumoJunction()
    {
        ID = TEXT("");
        Location = FVector2D::ZeroVector;
        Type = ESumoJunctionType::SJT_Unknown;
        Shape.Empty();
        IncomingLaneIDs.Empty();
        InternalLaneIDs.Empty();
        Requests.Empty();
    };
};
