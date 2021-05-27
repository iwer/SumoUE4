// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"

#include "SumoTrafficLightPhase.generated.h"

UENUM(BlueprintType)
enum ESumoTrafficLightSignalState {
    STLSS_Red UMETA(ToolTip = "red light for a signal - vehicles must stop"),
    STLSS_Yellow UMETA(ToolTip = "amber (yellow) light for a signal - vehicles will start to decelerate if far away from the junction, otherwise they pass"),
    STLSS_Green UMETA(ToolTip = "green light for a signal, no priority - vehicles may pass the junction if no vehicle uses a higher priorised foe stream, otherwise they decelerate for letting it pass."),
    STLSS_GreenPriority UMETA(ToolTip = "green light for a signal, priority - vehicles may pass the junction"),
    STLSS_RightGreen UMETA(ToolTip = "green right-turn arrow requires stopping - vehicles may pass the junction if no vehicle uses a higher priorised foe stream. They always stop before passing."),
    STLSS_RedYellow UMETA(ToolTip = "red+yellow light for a signal, may be used to indicate upcoming green phase but vehicles may not drive yet"),
    STLSS_OffBlink UMETA(ToolTip = "off - blinking signal is switched off, blinking light indicates vehicles have to yield"),
    STLSS_Off UMETA(ToolTip = "off - no signal signal is switched off, vehicles have the right of way")
};

USTRUCT(Blueprintable, BlueprintType)
struct FSumoTrafficLightPhase {
    GENERATED_BODY()

    /**
     * The duration of the phase
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Duration;

    /**
     * The traffic light states for this phase
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<TEnumAsByte<ESumoTrafficLightSignalState>> States;

    /**
     * The minimum duration of the phase when using type actuated. Optional, defaults to duration.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int MinDuration;

    /**
     * The maximum duration of the phase when using type actuated. Optional, defaults to duration.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int MaxDuration;

    /**
     * An optional description for the phase. This can be used to establish the
     * correspondence between SUMO-phase-indexing and traffic engineering phase names.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Name;

    /**
     * The next phase in the cycle after the current. This is useful when adding
     * extra transition phases to a traffic light plan which are not part of
     * every cycle. Traffic lights of type 'actuated' can make use of a list of
     * indices for selecting among alternative successor phases.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    TArray<int> Next;

    FSumoTrafficLightPhase() {
        Duration = 0;
        States.Empty();
        MinDuration = 0;
        MaxDuration = 0;
        Name = TEXT("");
        Next.Empty();
    }
};
