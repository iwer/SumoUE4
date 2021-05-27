// Copyright (c) Iwer Petersen. All rights reserved.

#pragma once

#include "CoreMinimal.h"


#include "SumoRequest.generated.h"

USTRUCT(Blueprintable, BlueprintType)
struct FSumoRequest {
    GENERATED_BODY()

    /**
     * The index of the described connection in the right-of-way matrix
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    int Index;

    /**
     * A bitstring that describes for each connection whether it prohibits
     * un-decelerated passing of the intersection for vehicles at this
     * connection. The rightmost bit corresponds to index 0.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Response;

    /**
     *
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    FString Foes;

    /**
     * A bitstring that describes for each connection whether it conflicts
     * with this connection. The rightmost bit corresponds to index 0.
     */
    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool bContinue;

    FSumoRequest() {
        Index = 0;
        Response = TEXT("");
        Foes = TEXT("");
        bContinue = false;
    }
};
