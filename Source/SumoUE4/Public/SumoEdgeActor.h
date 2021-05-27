// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "SumoBaseActor.h"
#include "Components/SplineMeshComponent.h"
#include "SumoStructs/SumoEdge.h"
#include "SumoJunctionActor.h"

#include "SumoEdgeActor.generated.h"

UCLASS()
class SUMOUE4_API ASumoEdgeActor : public ASumoBaseActor
{
    GENERATED_BODY()
public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FSumoEdge EdgeData;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<USplineComponent*> Lanes;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<USplineMeshComponent*> LaneMeshes;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UStaticMesh * RoadProfileMesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bReparamSpline;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float SplineSectionLength;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap<FString, ASumoJunctionActor*> FromJunctions;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TMap<FString, ASumoJunctionActor*> ToJunctions;

    ASumoEdgeActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void OnConstruction(const FTransform &Transform) override;

private:
    void ConstructLanes();
    USplineComponent * ConstructBaseSpline(FSumoLane &lane);
    void ConstructLaneMesh(USplineComponent * basespline, float laneWidth);
    FVector DetermineEdgeGeoCenter();

};
