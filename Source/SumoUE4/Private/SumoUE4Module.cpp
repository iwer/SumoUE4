// Copyright Epic Games, Inc. All Rights Reserved.

#include "SumoUE4Module.h"

#define LOCTEXT_NAMESPACE "FSumoUE4Module"

void FSumoUE4Module::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FSumoUE4Module::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FSumoUE4Module, SumoUE4)
