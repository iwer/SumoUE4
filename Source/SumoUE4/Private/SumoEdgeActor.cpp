// Copyright (c) Iwer Petersen. All rights reserved.

#include "SumoEdgeActor.h"

ASumoEdgeActor::ASumoEdgeActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReparamSpline = false;
    SplineSectionLength = 1000; // 10m
}

// Called when the game starts or when spawned
void ASumoEdgeActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASumoEdgeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ASumoEdgeActor::OnConstruction(const FTransform &Transform)
{
    FVector center = DetermineEdgeGeoCenter();
    GeoLocation->GeoRef->Longitude = center.X;
    GeoLocation->GeoRef->Latitude = center.Y;

    // Construct parent after Lat/Lon is set
    Super::OnConstruction(Transform);

    // cleanup
    for(auto &lane : Lanes) {
        if(lane)
            lane->DestroyComponent();
    }
    Lanes.Empty();
    // clear current meshes
    for(auto &lanemesh : LaneMeshes) {
        if(lanemesh) {
            lanemesh->DestroyComponent(true);
        }
    }
    LaneMeshes.Empty();

    if(EdgeData.Function == ESumoEdgeFunction::SEF_Normal) {
        if(EdgeData.Lanes.Num() > 0) {
            ConstructLanes();
        } else {
            UE_LOG(LogTemp, Warning, TEXT("ASumoEdgeActor: EdgeData has no lanes!"))
        }
    } else {
        UE_LOG(LogTemp, Warning, TEXT("ASumoEdgeActor: EdgeData not SEF_Normal!"))
    }

}

void ASumoEdgeActor::ConstructLanes()
{
    // do nothing when no georef is present
    if(!GeoLocation->GeoRef)
        return;

    // each lane
    for(auto &l : EdgeData.Lanes) {
        auto basespline = ConstructBaseSpline(l);
        if(basespline) {
            Lanes.Add(basespline);
            float width = EdgeData.Type.Width;
            if(width < .01)
                width = 3 * EdgeData.Type.NumLanes;
            ConstructLaneMesh(basespline, width * 100);
        }
    }
}

USplineComponent * ASumoEdgeActor::ConstructBaseSpline(FSumoLane &lane)
{
    USplineComponent * spline = ConstructSpline(lane.Shape, true, 10);
    // snap first spline point to fromJunction z-location
    if(SumoNetwork) {
        auto startjunction = FromJunctions[EdgeData.FromJunction];
        
    }
    // snap last spline point to toJunction z-location

    return spline;
}

void ASumoEdgeActor::ConstructLaneMesh(USplineComponent * basespline, float laneWidth)
{
    if(bReparamSpline) {
        // calculate reparam steps
        float splinelength = basespline->GetSplineLength();
        int numReparam = splinelength / SplineSectionLength;

        // create new vertices along spline
        TArray<FVector> newVerts;
        for(int i = 0; i < numReparam; i++) {
            FVector rp = basespline->GetLocationAtDistanceAlongSpline(i * SplineSectionLength, ESplineCoordinateSpace::Local);
            newVerts.Add(GeoLocation->SnapToGround(rp, 100));
        }

        // add end vertex
        FVector ev = basespline->GetLocationAtDistanceAlongSpline(splinelength, ESplineCoordinateSpace::Local);
        newVerts.Add(GeoLocation->SnapToGround(ev, 100));

        // clear spline points in basespline
        basespline->ClearSplinePoints(true);

        // add vertices
        for(auto v : newVerts) {
            basespline->AddSplineLocalPoint(v);
        }

        // update spline
        basespline->UpdateSpline();
    } else {
        SplineSectionLength = basespline->GetSplineLength();
    }

    // determine number of mesh sections
    int numMeshSections = basespline->GetNumberOfSplinePoints() - 1;

    for (int i = 0; i < numMeshSections; i++) {
        FVector start = basespline->GetLocationAtSplinePoint(i,ESplineCoordinateSpace::World);
        FVector end = basespline->GetLocationAtSplinePoint(i+1,ESplineCoordinateSpace::World);
        FVector sTangent = basespline->GetTangentAtSplinePoint(i,ESplineCoordinateSpace::World);
        FVector eTangent = basespline->GetTangentAtSplinePoint(i+1,ESplineCoordinateSpace::World);

        USplineMeshComponent * mesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
        mesh->SetStartAndEnd(start, sTangent, end, eTangent, true);
        mesh->SetSplineUpDir(FVector(0,0,1),true);
        mesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        mesh->SetForwardAxis(ESplineMeshAxis::Type::X, true);

        // attach static mesh
        if(RoadProfileMesh) {
            mesh->SetStaticMesh(RoadProfileMesh);
        }

        mesh->RegisterComponent();
        LaneMeshes.Add(mesh);
    }
}

FVector ASumoEdgeActor::DetermineEdgeGeoCenter()
{
    FVector center = FVector::ZeroVector;
    float count = 0;
    for(auto &lane : EdgeData.Lanes){
        for(auto &v : lane.Shape){
            center += v;
            count+=1;
        }
    }

    if(count > 1)
        center = center / count;

    return center;
}
