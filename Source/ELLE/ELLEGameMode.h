// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.
#pragma once

#include "CoreMisc.h"
#include "TrackGenerator.h"
#include "StaticGameSettings.h"
#include "ModuleHandler.h"
#include "GameFramework/GameModeBase.h"
#include "ELLEGameMode.generated.h"

UCLASS(minimalapi, config=ELLEDefaults)
class AELLEGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AELLEGameMode();
	~AELLEGameMode();

	virtual void Tick(float dt) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = LanguageModules)
	struct FLanguageModule GetModule();


	UFUNCTION(BlueprintCallable, Category = LanguageModules)
	void InitTest();

	UFUNCTION(BlueprintCallable, Category = LanguageModules)
	struct FTermPackage GetNextTerm();

	//UFUNCTION(BlueprintCallable, Category = LanguageModules)
	//struct FTermPackage GetTermPackage();

	UFUNCTION(BlueprintCallable, Category = Settings)
	TEnumAsByte<enum EGameType> GetGameType();
	UFUNCTION(BlueprintCallable, Category = Settings)
	TEnumAsByte<enum EDifficultyType> GetDifficulty();
	UFUNCTION(BlueprintCallable, Category = Settings)
	int GetNumberOfPuzzleOptions();
	UFUNCTION(BlueprintCallable, Category = Settings)
	bool IsTesting();

	UFUNCTION(BlueprintCallable, Category = RunRecording)
	void RecordPuzzle(bool isCorrect, FString focus, TArray<FString> options, EPuzzleType puzzleType);

	UFUNCTION(BlueprintCallable, Category = RunRecording)
	TArray<FPuzzleObject> GetPuzzles();

	UFUNCTION(BlueprintCallable, Category = RunRecording)
	void CloseRunFile();

	UFUNCTION(BlueprintCallable, Category = TrackGeneration)
	void AddInitialTrackChunk(TArray<ATrackPiece*> track);
	UFUNCTION(BlueprintCallable, Category = TrackGeneration)
	void GenerateNextTrackChunk();

	UFUNCTION(BlueprintCallable, Category = Score)
	void AddToScore(int points);

	UFUNCTION(BlueprintCallable, Category = RunRecording)
	int GetScore();

	UFUNCTION(BlueprintCallable, Category = TermStuff)
	UTexture2D* GetTermImage(FTerm trm);

	UFUNCTION(BlueprintCallable, Category = TermStuff)
	USoundWave* GetTermAudio(FTerm trm, bool isTranslated);

	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetMusicVolume(float val);
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetPuzzleVolume(float val);
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetSFXVolume(float val);
	UFUNCTION(BlueprintCallable, Category = Settings)
	void SetBrightnessValue(float val);

	UFUNCTION(BlueprintCallable, Category = Settings)
	float GetMusicVolume();
	UFUNCTION(BlueprintCallable, Category = Settings)
	float GetPuzzleVolume();
	UFUNCTION(BlueprintCallable, Category = Settings)
	float GetSFXVolume();
	UFUNCTION(BlueprintCallable, Category = Settings)
	float GetBrightnessValue();

	UFUNCTION(BlueprintCallable, Category = Settings)
	bool GetIsEnglishBase();


private:

	ModuleHandler moduleHandler;

	class APawn* character;
	FVector PlayerPosition;

	//for block selection
	TArray<FString> words;

	TrackGenerator* trackGen;


};



