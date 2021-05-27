// Copyright (c) Iwer Petersen. All rights reserved.
#include "Modules/ModuleManager.h"
#include "Toolkits/AssetEditorToolkit.h"


#define LOCTEXT_NAMESPACE "FSumoUE4EditorModule"

class FSumoUE4EditorModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override
	{
        // Style = MakeShareable(new FGeotiffHeightmapAssetEditorStyle());

		RegisterAssetTools();
    }

	virtual void ShutdownModule() override
	{
        UnregisterAssetTools();
        //UnregisterLandscapeFileFormats();
	}

    virtual bool SupportsDynamicReloading() override
	{
		return true;
	}
protected:
    /** Registers asset tool actions. */
    void RegisterAssetTools()
    {
        // IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        // TSharedPtr<IAssetTypeActions> heightActionType = MakeShareable(new FGeotiffHeightmapAssetActions(Style.ToSharedRef()));
        // RegisterAssetTypeAction(AssetTools, heightActionType.ToSharedRef());
        // TSharedPtr<IAssetTypeActions> weightActionType = MakeShareable(new FS2GLCWeightmapAssetActions(Style.ToSharedRef()));
        // RegisterAssetTypeAction(AssetTools, weightActionType.ToSharedRef());

    }

    /**
	 * Registers a single asset type action.
	 *
	 * @param AssetTools The asset tools object to register with.
	 * @param Action The asset type action to register.
	 */
	// void RegisterAssetTypeAction(IAssetTools& AssetTools, TSharedRef<IAssetTypeActions> Action)
	// {
	// 	// AssetTools.RegisterAssetTypeActions(Action);
	// 	// RegisteredAssetTypeActions.Add(Action);
	// }

    /** Unregisters asset tool actions. */
	void UnregisterAssetTools()
	{
		// FAssetToolsModule* AssetToolsModule = FModuleManager::GetModulePtr<FAssetToolsModule>("AssetTools");
        //
		// if (AssetToolsModule)
		// {
		// 	IAssetTools& AssetTools = AssetToolsModule->Get();
        //
		// 	for (auto Action : RegisteredAssetTypeActions)
		// 	{
		// 		AssetTools.UnregisterAssetTypeActions(Action);
		// 	}
		// }
	}


private:
    /** The collection of registered asset type actions. */
    // TArray<TSharedRef<IAssetTypeActions>> RegisteredAssetTypeActions;
    /** Holds the plug-ins style set. */
    // TSharedPtr<ISlateStyle> Style;
};

IMPLEMENT_MODULE(FSumoUE4EditorModule, SumoUE4Editor);
#undef LOCTEXT_NAMESPACE
