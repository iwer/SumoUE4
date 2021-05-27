// Copyright (c) Iwer Petersen. All rights reserved.
#pragma once

#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"

#include "SumoNetworkAssetFactory.generated.h"

UCLASS(BlueprintType, hidecategories=Object)
class USumoNetworkAssetFactory
    : public UFactory
{
    GENERATED_BODY()
public:
    USumoNetworkAssetFactory(const FObjectInitializer& ObjectInitializer);
	virtual UObject* FactoryCreateFile(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, const FString& Filename, const TCHAR* Parms, FFeedbackContext* Warn, bool& bOutOperationCanceled) override;
    virtual bool FactoryCanImport(const FString & Filename) override;
};
