// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "StaticGameSettings.h"

/**
 * 
 */
class ELLE_API ModuleHandler
{
public:
	ModuleHandler();
	~ModuleHandler();

	TArray<FLanguageModule> Modules;

	FLanguageModule* GetRandomModule();

	void LoadModules(bool isShallow);
	void ParseJSONModule_Shallow(FLanguageModule* langMod, FString rawJson);
	void ParseJSONModule(FLanguageModule* langMod, FString rawJson);

};
