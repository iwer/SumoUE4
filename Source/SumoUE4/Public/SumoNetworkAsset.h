// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "Engine/DataAsset.h"
#include "SumoStructs/SumoJunction.h"
#include "SumoStructs/SumoEdge.h"
#include "SumoStructs/SumoRoadType.h"
#include "SumoStructs/SumoTrafficLightProgram.h"
#include "SumoStructs/SumoConnection.h"

#include "SumoNetworkAsset.generated.h"
/**
 *
 */


UCLASS(BlueprintType, hidecategories=(Object))
class SUMOUE4_API USumoNetworkAsset : public UDataAsset
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly,EditAnywhere)
    TMap<FString, FSumoEdge> Edges;

    UPROPERTY(BlueprintReadOnly,EditAnywhere)
    TMap<FString, FSumoJunction> Junctions;

    UPROPERTY(BlueprintReadOnly,EditAnywhere)
    TMap<FString, FSumoRoadType> RoadTypes;

    UPROPERTY(BlueprintReadOnly,EditAnywhere)
    TMap<FString, FSumoTrafficLightProgram> TrafficLightPrograms;

    UPROPERTY(BlueprintReadOnly, EditAnywhere)
    TArray<FSumoConnection> Connections;
};
