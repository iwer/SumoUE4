// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SumoVehicleType.h"

#include "SumoStopOffset.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FSumoStopOffset {
    GENERATED_BODY()

    /**
     * The stop offset as positive value in meters.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Value;

    /**
     * Specifies, for which vehicle classes the stopOffset applies.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<TEnumAsByte<ESumoVehicleType>> VehicleClasses;

    /**
     * Specifies, for which vehicle classes the stopOffset does not apply.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<TEnumAsByte<ESumoVehicleType>> Exceptions;

    FSumoStopOffset() {
        Value = 0.0;
        VehicleClasses.Empty();
        Exceptions.Empty();
    }
};
