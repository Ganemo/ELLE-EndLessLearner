// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "ImageWrapper.h"
#include "MusicPlayerLibBPLibrary.h"
#include "StaticGameSettings.generated.h"

UENUM(BlueprintType)
enum EGameType
{
	ThirdPerson_Perpendicular,
	ThirdPerson_OverTheShoulder,
	FirstPerson_VR
};

UENUM(BlueprintType)
enum ERunType
{
	Endless,
	Test
};

UENUM(BlueprintType)
enum EDifficultyType
{
	Easy,
	Medium,
	Hard
};

USTRUCT(BlueprintType)
struct FModulePackage
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		FString Module;
	UPROPERTY(BlueprintReadWrite)
		int Version;
	UPROPERTY(BlueprintReadWrite)
		int Status = -1;
};

UENUM(BlueprintType)
enum EPuzzleType
{
	Text_Image,
	Audio_Image,
	Image_Text,
	Text_Text,
	Audio_Text
};

USTRUCT(BlueprintType)
struct FTerm
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		FString Term;
	UPROPERTY(BlueprintReadWrite)
		FString TermInEnglish;

	FString AudioPath;
	FString TranslatedAudioPath;
	FString ImagePath;


public:
	bool operator==(FTerm* termval) const
	{
		return termval->Term.Equals(this->Term);
	}

	USoundWave* GetAudio(const FString ModName)
	{
		return GetAudioFromFile(ModName, AudioPath);
	}

	USoundWave* GetTranslatedAudio(const FString ModName)
	{
		return GetAudioFromFile(ModName, TranslatedAudioPath);
	}

	USoundWave* GetAudioFromFile(const FString ModName, const FString path)
	{
		FString Path1, Filename1, Extension1;
		FPaths::Split(*(FPaths::GameSavedDir() + "LanguageModules/" + ModName + "/" + path), Path1, Filename1, Extension1);
		return UMusicPlayerLibBPLibrary::GetSoundWaveFromFile(*(Path1+"/"+Filename1+".ogg"));
	}

	UTexture2D* GetImage(const FString ModName)
	{
		UTexture2D* LoadedT2D = NULL;

		IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));

		IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);
		

		//Load From File *(ImagePath + ".png")
		TArray<uint8> RawFileData;

		if (!FFileHelper::LoadFileToArray(RawFileData, *(FPaths::GameSavedDir() + "LanguageModules/" + ModName + "/" + ImagePath)))
		{
			return NULL;
		}

		int Width;
		int Height;

		//Create T2D!
		if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
		{
			const TArray<uint8>* UncompressedBGRA = NULL;
			if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
			{
				LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

				//Valid?
				if (!LoadedT2D)
				{
					return NULL;
				}

				//Out!
				Width = ImageWrapper->GetWidth();
				Height = ImageWrapper->GetHeight();

				//Copy!
				void* TextureData = LoadedT2D->PlatformData->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
				FMemory::Memcpy(TextureData, UncompressedBGRA->GetData(), UncompressedBGRA->Num());
				LoadedT2D->PlatformData->Mips[0].BulkData.Unlock();

				//Update!
				LoadedT2D->UpdateResource();
			}
		}

		// Success!
		return LoadedT2D;
	}

	bool operator==(FTerm pc)
	{
		return Term == pc.Term;
	}
};

USTRUCT(BlueprintType)
struct FTermPackage
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		TEnumAsByte<EPuzzleType> type;

	UPROPERTY(BlueprintReadWrite)
		TArray<struct FTerm> terms;

	FTermPackage()
	{

	}

	FTermPackage(TEnumAsByte<EPuzzleType> _type, TArray<struct FTerm> _otherTerms)
	{
		type = _type;
		terms = _otherTerms;
	}

	bool operator==(FTermPackage pc)
	{
		if (pc.terms[0] == terms[0] && pc.type == type)
			return true;
		else
			return false;
	}

	void operator=(FTermPackage pc)
	{
		type = pc.type;
		terms.Empty();
		terms.Append(pc.terms);
	}
};

USTRUCT(BlueprintType)
struct FLanguageModule
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite)
		FString ModuleName;
	UPROPERTY(BlueprintReadWrite)
		int VersionNumber;
	UPROPERTY(BlueprintReadWrite)
		TArray<FTerm> Terms;

	FTerm* GetRandomTerm(TArray<FTerm>* ExcludingTerms)
	{
		if (ExcludingTerms == NULL)
		{
			ExcludingTerms = new TArray<FTerm>();
		}

		int num = Terms.Num();
		FTerm* term = &Terms[rand() % num];

		int loopcap = 100;
		while (ExcludingTerms->Contains(term) && loopcap > 0)
		{
			UE_LOG(LogTemp, Log, TEXT("\tReturner Contains '%s'"), *term->Term);
			term = &Terms[rand() % num];
			loopcap--;
		}

		return term;
	}

	TArray<FTerm> GetRandomTermsOfCount(int count)
	{

		UE_LOG(LogTemp, Log, TEXT("Querrying for '%d' terms"), count);
		TArray<FTerm>* returner = new TArray<FTerm>();
		for (int x = 0; x < count; x++)
		{
			FTerm* term = GetRandomTerm(returner);
			returner->Add(*term);
		}

		return *returner;
	}
	TArray<FTerm> GetRandomTermsOfCountExcluding(int count, FTerm excluder)
	{

		UE_LOG(LogTemp, Log, TEXT("Querrying for '%d' terms"), count);
		TArray<FTerm>* returner = new TArray<FTerm>();
		returner->Add(excluder);
		while(returner->Num() < count)
		{
			FTerm* term = GetRandomTerm(returner);
			returner->Add(*term);
		}

		return *returner;
	}


	UPROPERTY(BlueprintReadWrite)
	TArray<FTermPackage> TestModule;

	FTermPackage GetRandomTextTermPackage()
	{
		if (TestModule.Num() > 0)
		{
			int rand = FMath::Rand() % TestModule.Num();
			FTermPackage pck = TestModule[rand];
			TestModule.RemoveAt(rand);
			return pck;
		}
		else
		{
			return FTermPackage();
		}
	}

};

UENUM(BlueprintType)
enum class EJoyImageFormats : uint8
{
	JPG		UMETA(DisplayName = "JPG        "),
	PNG		UMETA(DisplayName = "PNG        "),
	BMP		UMETA(DisplayName = "BMP        "),
	ICO		UMETA(DisplayName = "ICO        "),
	EXR		UMETA(DisplayName = "EXR        "),
	ICNS		UMETA(DisplayName = "ICNS        ")
};

/**
* Possible results from a download request.
*/
UENUM(BlueprintType, Category = "HTTP")
enum class EDownloadResult : uint8
{
	Success,
	RequestFailed,
	DownloadFailed,
	SaveFailed,
	DirectoryCreationFailed
};

enum ETrackPiece_Type
{
	Normal,
	Puzzle,
	Avoidance,
	Start,
	End
};

struct FTrackPiece
{
	ETrackPiece_Type type;
	int pos_x = 0;
	int pos_y = 0;

	FTrackPiece(ETrackPiece_Type t, int x, int y)
	{
		type = t;
		pos_x = x;
		pos_y = y;
	}
};

USTRUCT(BlueprintType)
struct FPuzzleObject
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadOnly)
		TEnumAsByte<EPuzzleType> Type;

	UPROPERTY(BlueprintReadOnly)
		FString Focus;
	UPROPERTY(BlueprintReadOnly)
		TArray<FString> Options;

	UPROPERTY(BlueprintReadOnly)
		bool CorrectlySolved;
};




class ELLE_API StaticGameSettings
{
public:

	static FString userName;

	//endless/training or test
	static TEnumAsByte<EGameType> gameType;
	//third person, third person over the shoulder, first person VR
	static TEnumAsByte<ERunType> runType;

	//language module selected
	static FLanguageModule selectedLanguage;

	static bool isEnglishBase;

	/*Difficulty Settings*/
	static TEnumAsByte<EDifficultyType> difficultyType;
	//how many choices are there per puzzle
	static int numberOfPuzzleOptions;
	//how many blocks between puzzles
	static int puzzleInterval;
	//the chance an obstacle block will spawn
	static float obstacleSpawnChance;
	//the total size of the track (in testing mode)
	static int trackSize;

	static bool isTestMode;

	static FString hatBPPath;
	static FColor bodyColor;

	static int isLoggedIn;
	static FString JWTToken;

	static TArray<FModulePackage> servermodules;
	static class RunRecorderHandler* runRecorderHandler;

	static float musicVolume;
	static float puzzleVolume;
	static float sfxVolume;
	static float brightnessValue;

	static void GetAllFilesInDirectory(TArray<FString>* files, FString directory, bool fullPath, FString onlyFilesStartingWith, FString onlyFilesWithExtension);
	static UTexture2D* LoadTexture2DFromFile(const FString& FullFilePath, EJoyImageFormats ImageFormat, bool& IsValid);
	static void SetDifficulty(TEnumAsByte<EDifficultyType> type);
	static bool SetGameType(TEnumAsByte<EGameType> type);
};
