// Copyright (c) Iwer Petersen. All rights reserved.

#include "SumoBaseActor.h"

ASumoBaseActor::ASumoBaseActor()
{
    PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ASumoBaseActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASumoBaseActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASumoBaseActor::OnConstruction(const FTransform &Transform)
{
    // Construct parent after Lat/Lon is set
    Super::OnConstruction(Transform);
}

USplineComponent * ASumoBaseActor::ConstructSpline(TArray<FVector> &shape, bool bPutToGround, float groundOffset)
{
    if(shape.Num() >= 2) {
        // create spline component for lane
        USplineComponent * currspline = NewObject<USplineComponent>(this, USplineComponent::StaticClass());
        currspline->ClearSplinePoints(true);

        for(auto &v : shape) {
            FVector gv = GeoLocation->GeoRef->ToGameCoordinate(FVector(v.X, v.Y, GetActorLocation().Z));

            // snap to ground
            if(bPutToGround)
                gv = GeoLocation->SnapToGround(gv, 1000);

            // make relative to origin
            gv -= GetActorLocation();

            // offset
            gv += FVector(0,0,groundOffset);

            // add spline point
            currspline->AddSplineLocalPoint(gv);
        }

        currspline->UpdateSpline();
        currspline->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
        currspline->RegisterComponent();
        return currspline;
    }
    return nullptr;
}
