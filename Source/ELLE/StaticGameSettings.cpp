// Fill out your copyright notice in the Description page of Project Settings.

#include "ELLE.h"
#include "StaticGameSettings.h"

TEnumAsByte<EGameType> StaticGameSettings::gameType = EGameType::ThirdPerson_Perpendicular;
TEnumAsByte<ERunType> StaticGameSettings::runType = ERunType::Endless;
TEnumAsByte<EDifficultyType> StaticGameSettings::difficultyType = EDifficultyType::Easy;

int StaticGameSettings::trackSize = 100;
int StaticGameSettings::puzzleInterval = 10;
float StaticGameSettings::obstacleSpawnChance = 0.4f;

int StaticGameSettings::numberOfPuzzleOptions = 3;

FLanguageModule StaticGameSettings::selectedLanguage;
bool StaticGameSettings::isEnglishBase = true;

bool StaticGameSettings::isTestMode = false;

FString StaticGameSettings::hatBPPath = TEXT("");
FColor StaticGameSettings::bodyColor = FColor(1, 1, 1, 1);

FString StaticGameSettings::userName = "InvalidRunName";

FString StaticGameSettings::JWTToken = "";

int StaticGameSettings::isLoggedIn = -1;

TArray<FModulePackage> StaticGameSettings::servermodules;

RunRecorderHandler* StaticGameSettings::runRecorderHandler;

float StaticGameSettings::musicVolume = .5;
float StaticGameSettings::puzzleVolume = 1;
float StaticGameSettings::sfxVolume = .5;
float StaticGameSettings::brightnessValue = 1;

/// <summary>
/// Set the current difficulty and associated settings
/// </summary>
/// <param name="type"></param>
void StaticGameSettings::SetDifficulty(TEnumAsByte<enum EDifficultyType> type)
{
	switch (type)
	{
	case EDifficultyType::Easy:
		StaticGameSettings::numberOfPuzzleOptions = 3;
		StaticGameSettings::puzzleInterval = 10;
		StaticGameSettings::obstacleSpawnChance = 0.4f;
		break;
	case EDifficultyType::Medium:
		StaticGameSettings::numberOfPuzzleOptions = 4;
		StaticGameSettings::puzzleInterval = 8;
		StaticGameSettings::obstacleSpawnChance = 0.6f;
		break;
	case EDifficultyType::Hard:
		StaticGameSettings::numberOfPuzzleOptions = 5;
		StaticGameSettings::puzzleInterval = 6;
		StaticGameSettings::obstacleSpawnChance = 0.6f;
		break;
	default:
		break;
	}

	StaticGameSettings::difficultyType = type;
}
/// <summary>
/// Set the game type, doing validation checks for Head Mounted Display for the VR mode
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
bool StaticGameSettings::SetGameType(TEnumAsByte<EGameType> type)
{
	if (type.GetValue() == EGameType::FirstPerson_VR)
	{
		if (GEngine && GEngine->HMDDevice.IsValid())
		{
			gameType = type;
			return true;
		}
		else
			return false;
	}
	else
	{
		gameType = type;
		return true;
	}
}
/// <summary>
/// Get all files in a directory
/// </summary>
/// <param name="files"></param>
/// <param name="directory"></param>
/// <param name="fullPath"></param>
/// <param name="onlyFilesStartingWith"></param>
/// <param name="onlyFilesWithExtension"></param>
void StaticGameSettings::GetAllFilesInDirectory(TArray<FString>* files, FString directory, bool fullPath, FString onlyFilesStartingWith, FString onlyFilesWithExtension)
{
	// Get all files in directory
	TArray<FString> directoriesToSkip;
	IPlatformFile &PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	FLocalTimestampDirectoryVisitor Visitor(PlatformFile, directoriesToSkip, directoriesToSkip, false);
	PlatformFile.IterateDirectory(*directory, Visitor);

	for (TMap<FString, FDateTime>::TIterator TimestampIt(Visitor.FileTimes); TimestampIt; ++TimestampIt)
	{
		const FString filePath = TimestampIt.Key();
		const FString fileName = FPaths::GetCleanFilename(filePath);
		bool shouldAddFile = true;

		// Check if filename starts with required characters
		if (!onlyFilesStartingWith.IsEmpty())
		{
			const FString left = fileName.Left(onlyFilesStartingWith.Len());

			if (!(fileName.Left(onlyFilesStartingWith.Len()).Equals(onlyFilesStartingWith)))
				shouldAddFile = false;
		}

		// Check if file extension is required characters
		if (!onlyFilesWithExtension.IsEmpty())
		{
			if (!(FPaths::GetExtension(fileName, false).Equals(onlyFilesWithExtension, ESearchCase::IgnoreCase)))
				shouldAddFile = false;
		}

		// Add full path to results
		if (shouldAddFile)
		{
			files->Add(fullPath ? filePath : fileName);
		}
	}
}