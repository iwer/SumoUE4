// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SumoStopOffset.h"
#include "SumoLane.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FSumoLane {
    GENERATED_BODY()

    /**
     * The id of the lane
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ID;

    /**
     * A running number, starting with zero at the right-most lane
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Index;

    /**
     * The maximum speed allowed on this lane [m/s]
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Speed;

    /**
     * The length of this lane [m]
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float Length;

    /**
     *
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FSumoStopOffset StopOffset;

    /**
     * The geometry of the lane, given by a polyline that describes the lane's
     * center line; must not be empty or have less than two positions
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FVector> Shape;

    FSumoLane() {
        ID = TEXT("");
        Index = 0;
        Speed = 0.0;
        Length = 0.1;
        Shape.Empty();
    }
};
