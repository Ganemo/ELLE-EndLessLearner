// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "StaticGameSettings.h"
#include "ModuleHandler.h"
#include "HTTPRequestHandler.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"



/**
 * 
 */
UCLASS()
class ELLE_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	AMainMenuGameMode();

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		TEnumAsByte<EGameType> GetGameType();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		bool SetGameType(TEnumAsByte<EGameType> type);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		TEnumAsByte<ERunType> GetRunType();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetRunType(TEnumAsByte<ERunType> type);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		TArray<FString> GetAllAvailableLanguages();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		FString GetSelectedLanguage();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetSelectedLanguage(FString string);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		bool GetIsEnglishBase();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetIsEnglishBase(bool b);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		TEnumAsByte<EDifficultyType> GetDifficulty();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetDifficulty(TEnumAsByte<EDifficultyType> type);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		int GetNumberOfPuzzleOptions();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetNumberOfPuzzleOptions(int num);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		int GetPuzzleInterval();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetPuzzleInterval(int num);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		float GetObstacleSpawnChance();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetObstacleSpawnChance(float num);
	
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		int GetTrackSize();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetTrackSize(int num);

	UFUNCTION(BlueprintCallable, Category = RunFile)
		void SetRunFileName(FString name);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		FString GetHatBPPath();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetHatBPPath(FString str);
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetHatBPPathFromObject(UStaticMeshComponent* cmpt);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		FColor GetBodyColor();
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetBodyColor(FColor color);

	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		void SetIsTesting(bool b);
	UFUNCTION(BlueprintCallable, Category = TrackSpawning)
		bool GetIsTesting();

	UFUNCTION(BlueprintCallable, Category = HTTP)
		UHTTPRequestHandler* MakeLogInRequest(FString usr, FString pw);

	UFUNCTION(BlueprintCallable, Category = HTTP)
		int CheckIfLoggedIn();

	UFUNCTION(BlueprintCallable, Category = HTTP)
		UHTTPRequestHandler* MakeAccountCreationPost(FString username, FString pw, FString email, FString sex, int age, FString security, FString motivation);
	UFUNCTION(BlueprintCallable, Category = HTTP)
		UHTTPRequestHandler* MakePasswordResetPost(FString username, FString email, FString resetquestion, FString newpassword);

	UFUNCTION(BlueprintCallable, Category = HTTP)
		TArray<FModulePackage> GetServerModules();
	UFUNCTION(BlueprintCallable, Category = HTTP)
		UHTTPRequestHandler* GetModulesFromServer();

	UFUNCTION(BlueprintCallable, Category = RunRecording)
		TArray<struct FPuzzleObject> GetPuzzles();
	UFUNCTION(BlueprintCallable, Category = RunRecording)
		int GetScore();

	UFUNCTION(BlueprintCallable, Category = Modules)
		void LoadModules(bool shallowload);
	UFUNCTION(BlueprintCallable, Category = Modules)
		UHTTPRequestHandler* DownloadModule(const FString str);

	ModuleHandler moduleHandler;

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
};
