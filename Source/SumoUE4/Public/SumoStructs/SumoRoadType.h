// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SumoVehicleType.h"

#include "SumoRoadType.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FSumoRoadType {
    GENERATED_BODY()

    /**
     * The name of the road type. This is the only mandatory attribute. For
     * OpenStreetMap data, the name could, for example, be highway.trunk or
     * highway.residential. For ArcView data, the name of the road type is a
     * number.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ID;

    /**
     * A number, which determines the priority between different road types.
     * netconvert derives the right-of-way rules at junctions from the priority.
     * The number starts with one; higher numbers represent more important roads.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Priority;

    /**
     * A number, which determines the priority between different road types.
     * netconvert derives the right-of-way rules at junctions from the priority.
     * The number starts with one; higher numbers represent more important roads.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int NumLanes;

    /**
     * The default (implicit) speed limit in m/s.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Speed;

    /**
     * List of allowed vehicle classes.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<TEnumAsByte<ESumoVehicleType>> AllowedVehicleTypes;

    /**
     * List of not allowed vehicle classes.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<TEnumAsByte<ESumoVehicleType>> DisallowedVehicleTypes;

    /**
     * True if this road type is only passable in one direction
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bOneWay;

    /**
     * The default width for added sidewalks (not so sure because actually named
     * <sidewalkWidth> in the docs but netconvert(v1_7_0+1227-e79d6b3592) spits
     * it out like this: <width>)
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Width;

    FSumoRoadType() {
        ID = TEXT("");
        Priority = 0;
        NumLanes = 1;
        Speed = 0.0;
        AllowedVehicleTypes.Empty();
        DisallowedVehicleTypes.Empty();
        bOneWay = false;
        Width = 0.0;
    }
};
