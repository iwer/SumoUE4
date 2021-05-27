// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "SumoVehicleType.h"
#include "SumoTrafficLightPhase.h"

#include "SumoTrafficLightProgram.generated.h"

UENUM(BlueprintType)
enum ESumoTrafficLightProgramType {
    STLPT_Static,
    STLPT_Actuated,
    STLPT_DelayBased
};

USTRUCT(Blueprintable, BlueprintType)
struct FSumoTrafficLightProgram {
    GENERATED_BODY()

    /**
     * The id of the traffic light. This must be an existing traffic light id
     * in the .net.xml file. Typically the id for a traffic light is identical
     * with the junction id. The name may be obtained by right-clicking the
     * red/green bars in front of a controlled intersection.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ID;

    /**
     * The type of the traffic light (fixed phase durations, phase prolongation
     * based on time gaps between vehicles (actuated), or on accumulated time
     * loss of queued vehicles (delay_based) )
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TEnumAsByte<ESumoTrafficLightProgramType> Type;

    /**
     * The id of the traffic light program; This must be a new program name for
     * the traffic light id. Please note that "off" is reserved
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString ProgramID;

    /**
     * The initial time offset of the program
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int TimeOffset;

    /**
     * The phases of this program
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<FSumoTrafficLightPhase> Phases;

    FSumoTrafficLightProgram() {
        ID = TEXT("");
        Type = ESumoTrafficLightProgramType::STLPT_Static;
        ProgramID = TEXT("");
        TimeOffset = 0;
    }
};
