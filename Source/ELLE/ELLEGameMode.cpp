// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "ELLE.h"
#include "RunRecorderHandler.h"
#include "ELLEGameMode.h"


AELLEGameMode::AELLEGameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	trackGen = new TrackGenerator();

	//// set default pawn class to our Blueprinted character
	ConstructorHelpers::FClassFinder<APawn> NormalPlayerClass(TEXT("/Game/Blueprints/Player/Runner"));
	ConstructorHelpers::FClassFinder<APawn> VRPlayerClass(TEXT("/Game/Blueprints/Player/VRRunner"));

	if (StaticGameSettings::gameType == EGameType::FirstPerson_VR)
	{
		if (VRPlayerClass.Class != NULL)
		{
			DefaultPawnClass = VRPlayerClass.Class;
		}
	}
	else
	{
		if (NormalPlayerClass.Class != NULL)
		{
			DefaultPawnClass = NormalPlayerClass.Class;
		}
	}


	InitTest();
}

AELLEGameMode::~AELLEGameMode()
{
}

/// <summary>
/// Function called when gamemode starts
/// </summary>
void AELLEGameMode::BeginPlay()
{
	Super::BeginPlay();

	
	StaticGameSettings::runRecorderHandler = new RunRecorderHandler();

	FString runtype;
	switch (StaticGameSettings::gameType)
	{
	case EGameType::ThirdPerson_Perpendicular:
		runtype = "TPP";
		break;	
	case EGameType::ThirdPerson_OverTheShoulder:
		runtype = "TPOS";
		break;
	case EGameType::FirstPerson_VR:
		runtype = "VR";
		break;
	default:
		runtype = "ERROR";
		break;
	}
	StaticGameSettings::runRecorderHandler->OpenRunFile(StaticGameSettings::userName + FString::FromInt(FDateTime::Now().ToUnixTimestamp()), runtype, FString::FromInt(StaticGameSettings::difficultyType), "Portuguese");

	character = GetWorld()->GetFirstPlayerController()->GetPawn();

}

/// <summary>
/// basic tick function
/// </summary>
/// <param name="dt"></param>
void AELLEGameMode::Tick(float dt)
{
	Super::Tick(dt);
}

/// <summary>
/// Get a language module
/// </summary>
struct FLanguageModule AELLEGameMode::GetModule()
{
	return StaticGameSettings::selectedLanguage;
}

/// <summary>
/// Get the next term package, handles testing mode
/// </summary>
struct FTermPackage AELLEGameMode::GetNextTerm()
{
	if (IsTesting())
	{
		return StaticGameSettings::selectedLanguage.GetRandomTextTermPackage();
	}
	else
	{
		FTermPackage pack;
		pack.terms = StaticGameSettings::selectedLanguage.GetRandomTermsOfCount(StaticGameSettings::numberOfPuzzleOptions);
		pack.type = static_cast<EPuzzleType>(FMath::Rand() % 5);

		return pack;
	}
}

/// <summary>
/// Create testing mode puzzles
/// </summary>
void AELLEGameMode::InitTest()
{
	if (IsTesting())
	{
		FLanguageModule* langmod = &StaticGameSettings::selectedLanguage;
		for (int x = 0; x < langmod->Terms.Num(); x++)
		{
			FTermPackage* pckg = new FTermPackage();
			pckg->type = EPuzzleType::Text_Text;
			pckg->terms = langmod->GetRandomTermsOfCountExcluding(StaticGameSettings::numberOfPuzzleOptions, langmod->Terms[x]);
			langmod->TestModule.Add(*pckg);

			FTermPackage* pckg2 = new FTermPackage();
			pckg2->type = EPuzzleType::Audio_Text;
			pckg2->terms = langmod->GetRandomTermsOfCountExcluding(StaticGameSettings::numberOfPuzzleOptions, langmod->Terms[x]);
			langmod->TestModule.Add(*pckg2);

			FTermPackage* pckg3 = new FTermPackage();
			pckg3->type = EPuzzleType::Image_Text;
			pckg3->terms = langmod->GetRandomTermsOfCountExcluding(StaticGameSettings::numberOfPuzzleOptions, langmod->Terms[x]);
			langmod->TestModule.Add(*pckg3);
		}
	}
}

/// <summary>
/// Get game type
/// </summary>
/// <returns></returns>
TEnumAsByte<enum EGameType> AELLEGameMode::GetGameType()
{
	return StaticGameSettings::gameType;
}
/// <summary>
/// Get difficulty
/// </summary>
/// <returns></returns>
TEnumAsByte<enum EDifficultyType> AELLEGameMode::GetDifficulty()
{
	return StaticGameSettings::difficultyType;
}
/// <summary>
/// Get puzzle options
/// </summary>
/// <returns></returns>
int AELLEGameMode::GetNumberOfPuzzleOptions()
{
	return StaticGameSettings::numberOfPuzzleOptions;
}
/// <summary>
/// Get whether testing mode is selected
/// </summary>
/// <returns></returns>
bool AELLEGameMode::IsTesting()
{
	return StaticGameSettings::isTestMode;
}

/// <summary>
/// Record a puzzle
/// </summary>
/// <param name="isCorrect"></param>
/// <param name="focus"></param>
/// <param name="options"></param>
/// <param name="puzzleType"></param>
void AELLEGameMode::RecordPuzzle(bool isCorrect, FString focus, TArray<FString> options, EPuzzleType puzzleType)
{
	StaticGameSettings::runRecorderHandler->AddPuzzle(isCorrect, focus, options, puzzleType);
}

/// <summary>
/// Get all recorded puzzles
/// </summary>
/// <returns></returns>
TArray<FPuzzleObject> AELLEGameMode::GetPuzzles()
{
	return StaticGameSettings::runRecorderHandler->puzzles;
}

/// <summary>
/// close a run recording
/// </summary>
void AELLEGameMode::CloseRunFile()
{
	StaticGameSettings::runRecorderHandler->CloseRunFile();
}

/// <summary>
/// Add the initial chunks of track when the run starts
/// </summary>
/// <param name="track"></param>
void AELLEGameMode::AddInitialTrackChunk(TArray<ATrackPiece*> track)
{
	trackGen->AnchorTracks.Insert(track, 0);

	trackGen->SpawnNextTrackChunk(GetWorld());
	trackGen->SpawnNextTrackChunk(GetWorld());
}
/// <summary>
/// Generate the next track chunk
/// </summary>
void AELLEGameMode::GenerateNextTrackChunk()
{
	trackGen->SpawnNextTrackChunk(GetWorld());
	trackGen->DestroyOldestTrackChunk();
}

/// <summary>
/// Add value to the score
/// </summary>
/// <param name="points"></param>
void AELLEGameMode::AddToScore(int points)
{
	StaticGameSettings::runRecorderHandler->score += points;
}

/// <summary>
/// Get the current score
/// return -1 if invalid
/// </summary>
/// <returns></returns>
int AELLEGameMode::GetScore()
{
	if (StaticGameSettings::runRecorderHandler != nullptr)
		return StaticGameSettings::runRecorderHandler->score;
	else
	{
		return -1;
	}
}

/// <summary>
/// Get the image from a term object
/// </summary>
/// <param name="trm"></param>
/// <returns></returns>
UTexture2D* AELLEGameMode::GetTermImage(FTerm trm)
{
	return trm.GetImage(StaticGameSettings::selectedLanguage.ModuleName);
}
/// <summary>
/// Get the audio file from a term object
/// </summary>
/// <param name="trm"></param>
/// <param name="isTranslated"></param>
/// <returns></returns>
USoundWave* AELLEGameMode::GetTermAudio(FTerm trm, bool isTranslated)
{
	if (isTranslated)
		return trm.GetTranslatedAudio(StaticGameSettings::selectedLanguage.ModuleName);
	else
		return trm.GetAudio(StaticGameSettings::selectedLanguage.ModuleName);
}
/// <summary>
/// Set the music volume
/// </summary>
/// <param name="val"></param>
void AELLEGameMode::SetMusicVolume(float val)
{
	StaticGameSettings::musicVolume = val;
}
/// <summary>
/// Set puzzle volume
/// </summary>
/// <param name="val"></param>
void AELLEGameMode::SetPuzzleVolume(float val)
{
	StaticGameSettings::puzzleVolume = val;
}
/// <summary>
/// Set sound effects volume
/// </summary>
/// <param name="val"></param>
void AELLEGameMode::SetSFXVolume(float val)
{
	StaticGameSettings::sfxVolume = val;
}
/// <summary>
/// DEPRICATED
/// </summary>
/// <param name="val"></param>
void AELLEGameMode::SetBrightnessValue(float val)
{
	StaticGameSettings::brightnessValue = val;
}

/// <summary>
/// Get music value
/// </summary>
/// <returns></returns>
float AELLEGameMode::GetMusicVolume()
{
	return StaticGameSettings::musicVolume;
}
/// <summary>
/// Get puzzle volume
/// </summary>
/// <returns></returns>
float AELLEGameMode::GetPuzzleVolume()
{
	return StaticGameSettings::puzzleVolume;
}
/// <summary>
/// Get sound effects volume
/// </summary>
/// <returns></returns>
float AELLEGameMode::GetSFXVolume()
{
	return StaticGameSettings::sfxVolume;
}
/// <summary>
/// DEPRICATED
/// </summary>
/// <returns></returns>
float AELLEGameMode::GetBrightnessValue()
{
	return StaticGameSettings::brightnessValue;
}
/// <summary>
/// Check whether english is chosen as the base language
/// </summary>
/// <returns></returns>
bool AELLEGameMode::GetIsEnglishBase()
{
	return StaticGameSettings::isEnglishBase;
}