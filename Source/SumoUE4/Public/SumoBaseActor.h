// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "GeoLocatedActor.h"
#include "SumoNetworkAsset.h"

#include "SumoBaseActor.generated.h"


UCLASS(Abstract)
class SUMOUE4_API ASumoBaseActor : public AGeoLocatedActor
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USumoNetworkAsset * SumoNetwork;

    ASumoBaseActor();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;
    virtual void OnConstruction(const FTransform &Transform) override;

    USplineComponent * ConstructSpline(TArray<FVector> &shape, bool bPutToGround, float groundOffset = 0);

};
