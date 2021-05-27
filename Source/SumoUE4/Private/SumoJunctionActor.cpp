// Copyright (c) Iwer Petersen. All rights reserved.


#include "SumoJunctionActor.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "PolygonHelper.h"


// Sets default values
ASumoJunctionActor::ASumoJunctionActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

    ProcMesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("ProcMesh"));
    PavementHeight = 10; // 10cm
}

// Called when the game starts or when spawned
void ASumoJunctionActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASumoJunctionActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ASumoJunctionActor::OnConstruction(const FTransform &Transform)
{
    GeoLocation->GeoRef->Longitude = JunctionData.Location.X;
    GeoLocation->GeoRef->Latitude = JunctionData.Location.Y;

    Super::OnConstruction(Transform);

    // cleanup
    for(auto &lane : LaneSplines) {
        if(lane)
            lane->DestroyComponent();
    }
    LaneSplines.Empty();

    ProcMesh->ClearAllMeshSections();

    ConstructMesh(JunctionData.Shape);
    ConstructInternalLanes();

}

void ASumoJunctionActor::ConstructMesh(TArray<FVector> &vertices)
{
    if(!GeoLocation->GeoRef) {
        UE_LOG(LogTemp, Warning, TEXT("ASumoJunctionActor: No GeoReference found, cancelling mesh construction!"))
        return;
    }

    MeshData data;
    data.OrigVertexNum = vertices.Num();

    // these we want to have, remember min & max
    float minX, maxX, minY, maxY, maxZ;
    minX = minY = std::numeric_limits<float>::max();
    maxX = maxY = maxZ = std::numeric_limits<float>::min();


    // make polygon in game coordinates
    for(auto &vertex : vertices) {
        // convert geocoords to gamecoords
        FVector gameloc = GeoLocation->GeoRef->ToGameCoordinate(FVector(vertex.X, vertex.Y, GetActorLocation().Z));

        // set Z to actorlocation
        FVector groundVert = GeoLocation->SnapToGround(gameloc,30);
        // subract actor location to make local coordinate
        gameloc -= GetActorLocation();
        // add pavement height
        gameloc += FVector(0,0,PavementHeight);

        // the vertices we'll use for the mesh
        data.Vertices.Add(gameloc);

        // remember min and max;
        minX = std::min(gameloc.X, minX);
        minY = std::min(gameloc.Y, minY);
        maxX = std::max(gameloc.X, maxX);
        maxY = std::max(gameloc.Y, maxY);
        maxZ = std::max(groundVert.Z-GetActorLocation().Z, maxZ);
    }

    // tesselate using earcut
    data.TriIndices = PolygonHelper::TesselatePolygon(data.Vertices, true);
    data.Normals.Init(FVector(0, 0, 1), data.Vertices.Num());
    data.Tangents.Init(FProcMeshTangent(1, 0, 0),data.Vertices.Num());
    data.VertColors.Init(FLinearColor(0, 0, 0, 1), data.Vertices.Num());
    
    // UV mapping 
    data.UV0 = PolygonHelper::FlatUVMap(data.Vertices);

    // create side mesh sections
    TArray<FVector> offsetVertices = PolygonHelper::GenerateOffsetVertices(data.Vertices, 40, PavementHeight);
    int baseVertNum = data.Vertices.Num();
    for (int32 i = 0; i < baseVertNum; i++) {
        auto p0 = data.Vertices[i];
        auto p1 = data.Vertices[(i + 1) % baseVertNum];
        auto p0a = offsetVertices[i];
        auto p1a = offsetVertices[(i + 1) % baseVertNum];
        AddQuadMeshSection(data, p0, p1, p0a, p1a);
    }

    // create the mesh section
    ProcMesh->CreateMeshSection_LinearColor(0, data.Vertices, data.TriIndices, data.Normals, data.UV0, data.VertColors, data.Tangents, false);
    ProcMesh->AddRelativeLocation(FVector(0,0,maxZ),false);
    ProcMesh->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
    //ProcMesh->RegisterComponent();
}


/**
 *  (n) p1     p2 (n+1)
 *       *-----*
 *      /       \
 * p1a *---------* p2a
 * (2n)          (2n+1)
 */
void ASumoJunctionActor::AddQuadMeshSection(MeshData &meshdata, const FVector &p1, const FVector &p2, const FVector &p1a, const FVector &p2a)
{
    int32 startIdx = meshdata.Vertices.Num();

    // make vertex array
    meshdata.Vertices.Add(p1); // <-- this is at startidx - meshdata.OrigVertexNum
    meshdata.Vertices.Add(p2); // <-- this is at startidx - meshdata.OrigVertexNum + 1
    meshdata.Vertices.Add(p1a);
    meshdata.Vertices.Add(p2a);

    // triangle p1, p2, p2a
    meshdata.TriIndices.Append({startIdx,
                                startIdx + 1,
                                startIdx + 3});
    // triangle p2a, p1a,p1
    meshdata.TriIndices.Append({startIdx + 3,
                                startIdx + 2,
                                startIdx});

    // calculate facenormal
    FVector a = p1 - p2;
    FVector b = p1 - p1a;
    FVector facenormal = FVector::CrossProduct(a,b).GetSafeNormal(.0001);
    FVector tangent = a.GetSafeNormal(.0001);


    // add normals, tangents and colors for each vertex
    TArray<FVector> normals;
    TArray<FLinearColor> colors;
    TArray<FProcMeshTangent> tangents;
    normals.Init(facenormal, 4);
    meshdata.Normals.Append(normals);
    tangents.Init(FProcMeshTangent(tangent, false), 4);
    meshdata.Tangents.Append(tangents);
    colors.Init(FLinearColor(0,0,0,1), 4);
    meshdata.VertColors.Append(colors);

    // calculate uvs by rotating the reference frame to the face normal along p1-p2
    // transforming the vertices to this frame which now lie in x-y plane
    // and scaling to meter which makes a good texture repetition scale
    FVector up(0,0,1);
    auto rotator = up.Rotation() - facenormal.Rotation();
    FVector uv1 = rotator.RotateVector(p1) / 100;
    FVector uv2 = rotator.RotateVector(p2) / 100;
    FVector uv1a = rotator.RotateVector(p1a) / 100;
    FVector uv2a = rotator.RotateVector(p2a) / 100;

    meshdata.UV0.Add(FVector2D(uv1));
    meshdata.UV0.Add(FVector2D(uv2));
    meshdata.UV0.Add(FVector2D(uv1a));
    meshdata.UV0.Add(FVector2D(uv2a));
}

void ASumoJunctionActor::ConstructInternalLanes()
{
    if(!SumoNetwork) {
        UE_LOG(LogTemp, Warning, TEXT("ASumoJunctionActor: Cannot construct internal lanes, SumoNetwork Asset not set!"));
        return;
    }

    for(FString &laneID : JunctionData.InternalLaneIDs) {
        // edgeID is laneID minus last "_<number>"
        int32 idLen;
        if(!laneID.FindLastChar('_', idLen)) {
            UE_LOG(LogTemp, Warning, TEXT("ASumoJunctionActor: Could not deduce Edge ID from LaneID %s"), *laneID);
            continue;
        }
        FString edgeID = laneID.Left(idLen);

        if(SumoNetwork->Edges.Contains(edgeID)) {
            auto edge = SumoNetwork->Edges[edgeID];
            FSumoLane lane;
            for(int i = 0; i < edge.Lanes.Num(); i++) {
                if(edge.Lanes[i].ID.Compare(laneID, ESearchCase::Type::CaseSensitive) == 0) {
                    lane = edge.Lanes[i];
                    break;
                }
            }
            // check if lane was found
            if(lane.ID.Compare(laneID, ESearchCase::Type::CaseSensitive) != 0) {
                UE_LOG(LogTemp, Warning, TEXT("ASumoJunctionActor: Lane %s not found in edge %s"), *laneID, *edgeID);
                break;
            }

            // construct spline
            auto spline = ConstructSpline(lane.Shape, false, PavementHeight);
            if(spline) {
                LaneSplines.Add(spline);
            }
        } else {
            UE_LOG(LogTemp, Warning, TEXT("ASumoJunctionActor: Edge %s not found in SumoNetwork!"), *edgeID);
        }
    }
}
