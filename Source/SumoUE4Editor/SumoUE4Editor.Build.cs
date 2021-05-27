// Copyright (c) Iwer Petersen. All rights reserved.

using UnrealBuildTool;

public class SumoUE4Editor : ModuleRules
{
	public SumoUE4Editor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
			});


		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
				"SumoUE4Editor/Private",
				"SumoUE4Editor/Private/Factories",
				"SumoUE4Editor/Private/Helpers",
//				"SumoUE4Editor/Private/AssetTools",
//				"SumoUE4Editor/Private/Landscape",
//				"SumoUE4Editor/Private/Styles"
			});


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// ... add private dependencies that you statically link with here ...
				"ContentBrowser",
				"Core",
				"CoreUObject",
				"DesktopWidgets",
				"EditorStyle",
				"Engine",
				"InputCore",
				"Projects",
                "RenderCore",
				"Slate",
				"SlateCore",
				"UnrealEd",
                "XmlParser",
                "GDAL",
                "UnrealGDAL",
                "GeoReference",
                "SumoUE4"
			});


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
                "AssetTools",
				"MainFrame"
			});

	    PrivateIncludePathModuleNames.AddRange(
			new string[] {
				"AssetTools",
				"UnrealEd"
			});
	}
}
