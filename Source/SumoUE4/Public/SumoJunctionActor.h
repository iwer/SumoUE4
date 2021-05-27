// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "SumoBaseActor.h"
#include "ProceduralMeshComponent.h"


#include "SumoJunctionActor.generated.h"

UCLASS()
class SUMOUE4_API ASumoJunctionActor : public ASumoBaseActor
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FSumoJunction JunctionData;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    UProceduralMeshComponent *ProcMesh;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    float PavementHeight;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<USplineComponent*> LaneSplines;

	// Sets default values for this actor's properties
	ASumoJunctionActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
    virtual void OnConstruction(const FTransform &Transform) override;
private:
    struct MeshData {
        int32 OrigVertexNum;
        TArray<FVector> Vertices;
        TArray<int32> TriIndices;
        TArray<FVector> Normals;
        TArray<FVector2D> UV0;
        TArray<FLinearColor> VertColors;
        TArray<FProcMeshTangent> Tangents;
    };
    void SetLocation(FVector2D & geocoordinates);
    void ConstructMesh(TArray<FVector> &vertices);
    void AddQuadMeshSection(MeshData &meshdata, const FVector &p1, const FVector &p2, const FVector &p1a, const FVector &p2a);

    void ConstructInternalLanes();

};
