// Copyright (c) Iwer Petersen. All rights reserved.

#include "SumoNetworkAssetFactory.h"
#include "SumoNetworkAsset.h"
#include "SumoNetworkParser.h"

USumoNetworkAssetFactory::USumoNetworkAssetFactory( const FObjectInitializer& ObjectInitializer )
	: Super(ObjectInitializer)
{
    SupportedClass = USumoNetworkAsset::StaticClass();
	bCreateNew = false;
	bEditorImport = true;
    Formats.Add(TEXT("xml;Sumo network definition file"));
}

UObject * USumoNetworkAssetFactory::FactoryCreateFile(UClass* InClass,
                                                      UObject* InParent,
                                                      FName InName,
                                                      EObjectFlags Flags,
                                                      const FString& Filename,
                                                      const TCHAR* Parms,
                                                      FFeedbackContext* Warn,
                                                      bool& bOutOperationCanceled)
{
    USumoNetworkAsset* Asset = NewObject<USumoNetworkAsset>(InParent, InClass, InName, Flags);

    // populate Asset
    USumoNetworkParser parser;
    parser.ParseFile(Filename, Asset);

    return Asset;
}

bool USumoNetworkAssetFactory::FactoryCanImport(const FString & Filename)
{
    if(!Filename.EndsWith(TEXT(".net.xml"))){
        return false;
    }
    return true;
}
