

#include "ELLE.h"
#include "StaticGameSettings.h"
#include "ModuleHandler.h"
#include "RunRecorderHandler.h"
#include "MainMenuGameMode.h"


AMainMenuGameMode::AMainMenuGameMode()
{
}

/// <summary>
/// Get current game type
/// </summary>
/// <returns></returns>
TEnumAsByte<EGameType> AMainMenuGameMode::GetGameType()
{
	return StaticGameSettings::gameType;
}
/// <summary>
/// Set current game type
/// </summary>
/// <param name="type"></param>
/// <returns></returns>
bool AMainMenuGameMode::SetGameType(TEnumAsByte<EGameType> type)
{
	return StaticGameSettings::SetGameType(type);
}
/// <summary>
/// Get current run type
/// </summary>
/// <returns></returns>
TEnumAsByte<ERunType> AMainMenuGameMode::GetRunType()
{
	return StaticGameSettings::runType;
}
/// <summary>
/// Set the current run type
/// </summary>
/// <param name="type"></param>
void AMainMenuGameMode::SetRunType(TEnumAsByte<ERunType> type)
{
	StaticGameSettings::runType = type;
}
/// <summary>
/// Get all downloaded modules
/// </summary>
/// <returns></returns>
TArray<FString> AMainMenuGameMode::GetAllAvailableLanguages()
{
	TArray<FString> mods;
	for (int x = 0; x<moduleHandler.Modules.Num(); x++)
	{
		mods.Add(moduleHandler.Modules[x].ModuleName);
	}
	return mods;
}
/// <summary>
/// Get the currently selected language
/// </summary>
/// <returns></returns>
FString AMainMenuGameMode::GetSelectedLanguage()
{
	return StaticGameSettings::selectedLanguage.ModuleName;
}
/// <summary>
/// Set the selected language by string name of langauge
/// </summary>
/// <param name="string"></param>
void AMainMenuGameMode::SetSelectedLanguage(FString string)
{
	for (int x=0; x<moduleHandler.Modules.Num(); x++)
	{
		if (moduleHandler.Modules[x].ModuleName.Equals(string))
		{
			StaticGameSettings::selectedLanguage = moduleHandler.Modules[x];
		}
	}
}
/// <summary>
/// Get whether english is selected as the base
/// </summary>
/// <returns></returns>
bool AMainMenuGameMode::GetIsEnglishBase()
{
	return StaticGameSettings::isEnglishBase;
}
/// <summary>
/// Set english as the selected base language
/// </summary>
/// <param name="b"></param>
void AMainMenuGameMode::SetIsEnglishBase(bool b)
{
	StaticGameSettings::isEnglishBase = b;
}
/// <summary>
/// Get the current difficulty
/// </summary>
/// <returns></returns>
TEnumAsByte<EDifficultyType> AMainMenuGameMode::GetDifficulty()
{
	return StaticGameSettings::difficultyType;
}
/// <summary>
/// Set the selected difficulty
/// </summary>
/// <param name="type"></param>
void AMainMenuGameMode::SetDifficulty(TEnumAsByte<EDifficultyType> type)
{
	StaticGameSettings::SetDifficulty(type);
}
/// <summary>
/// Get the number of puzzle
/// </summary>
/// <returns></returns>
int AMainMenuGameMode::GetNumberOfPuzzleOptions()
{
	return StaticGameSettings::numberOfPuzzleOptions;
}
/// <summary>
/// Set the number of puzzle options
/// </summary>
/// <param name="num"></param>
void AMainMenuGameMode::SetNumberOfPuzzleOptions(int num)
{
	StaticGameSettings::numberOfPuzzleOptions = num;
}
/// <summary>
/// Get the puzzle interval
/// </summary>
/// <returns></returns>
int AMainMenuGameMode::GetPuzzleInterval()
{
	return StaticGameSettings::puzzleInterval;
}
/// <summary>
/// Set the puzzle interval
/// </summary>
/// <param name="num"></param>
void AMainMenuGameMode::SetPuzzleInterval(int num)
{
	StaticGameSettings::puzzleInterval = num;
}
/// <summary>
/// Get the obstacle spawn chance
/// </summary>
/// <returns></returns>
float AMainMenuGameMode::GetObstacleSpawnChance()
{
	return StaticGameSettings::obstacleSpawnChance;
}
/// <summary>
/// Se the obstacle spawn chance
/// </summary>
/// <param name="num"></param>
void AMainMenuGameMode::SetObstacleSpawnChance(float num)
{
	StaticGameSettings::obstacleSpawnChance = num;
}
/// <summary>
/// Get the track size
/// </summary>
/// <returns></returns>
int AMainMenuGameMode::GetTrackSize()
{
	return StaticGameSettings::trackSize;
}
/// <summary>
/// Set the track size
/// </summary>
/// <param name="num"></param>
void AMainMenuGameMode::SetTrackSize(int num)
{
	StaticGameSettings::trackSize = num;
}
/// <summary>
/// Set the name of the run file
/// </summary>
/// <param name="name"></param>
void AMainMenuGameMode::SetRunFileName(FString name)
{
	StaticGameSettings::userName = name;
}
/// <summary>
/// Get the path of the BP of the hat for the character
/// </summary>
/// <returns></returns>
FString AMainMenuGameMode::GetHatBPPath()
{
	return StaticGameSettings::hatBPPath;
}
/// <summary>
/// Set the path of the BP of the hat for the character
/// </summary>
/// <param name="str"></param>
void AMainMenuGameMode::SetHatBPPath(FString str)
{
	StaticGameSettings::hatBPPath = str;
}
/// <summary>
///  the path of the BP of the hat for the character by getting it from a static mesh
/// </summary>
/// <param name="cmpt"></param>
void AMainMenuGameMode::SetHatBPPathFromObject(UStaticMeshComponent* cmpt)
{
	if (cmpt && cmpt->GetStaticMesh()->IsValidLowLevel())
		StaticGameSettings::hatBPPath = cmpt->GetStaticMesh()->GetPathName();
	else
		StaticGameSettings::hatBPPath = TEXT("");
}
/// <summary>
/// Get the current body color
/// </summary>
/// <returns></returns>
FColor AMainMenuGameMode::GetBodyColor()
{
	return StaticGameSettings::bodyColor;
}
/// <summary>
/// Set the current body color
/// </summary>
/// <param name="color"></param>
void AMainMenuGameMode::SetBodyColor(FColor color)
{
	StaticGameSettings::bodyColor = color;
}
/// <summary>
/// Set whether it is in testing mode
/// </summary>
/// <param name="b"></param>
void AMainMenuGameMode::SetIsTesting(bool b)
{
	StaticGameSettings::isTestMode = b;
}
/// <summary>
/// Get whether it is in testing mode
/// </summary>
/// <returns></returns>
bool AMainMenuGameMode::GetIsTesting()
{
	return StaticGameSettings::isTestMode;
}
/// <summary>
/// Try to log in
/// </summary>
/// <param name="usr"></param>
/// <param name="pw"></param>
/// <returns></returns>
UHTTPRequestHandler* AMainMenuGameMode::MakeLogInRequest(FString usr, FString pw)
{
	UHTTPRequestHandler* httpHandler = UHTTPRequestHandler::MakeHTTPRequester();
	httpHandler->MakeLogInRequest("159.203.163.147/auth", usr, pw);

	return httpHandler;
}
/// <summary>
/// Check if the user is currently logged in or in offline mode
/// </summary>
/// <returns></returns>
int AMainMenuGameMode::CheckIfLoggedIn()
{
	return StaticGameSettings::isLoggedIn;
}
/// <summary>
/// Attempt to create an account
/// </summary>
/// <param name="username"></param>
/// <param name="pw"></param>
/// <param name="email"></param>
/// <param name="sex"></param>
/// <param name="age"></param>
/// <param name="security"></param>
/// <param name="motivation"></param>
/// <returns></returns>
UHTTPRequestHandler* AMainMenuGameMode::MakeAccountCreationPost(FString username, FString pw, FString email, FString sex, int age, FString security, FString motivation)
{
	UHTTPRequestHandler* httpHandler = UHTTPRequestHandler::MakeHTTPRequester();
	httpHandler->MakeAccountCreationRequest("159.203.163.147/register", username, pw, email, sex, age, security, motivation);

	return httpHandler;
}
/// <summary>
/// Attempt to reset a user password
/// </summary>
/// <param name="username"></param>
/// <param name="email"></param>
/// <param name="resetquestion"></param>
/// <param name="newpassword"></param>
/// <returns></returns>
UHTTPRequestHandler* AMainMenuGameMode::MakePasswordResetPost(FString username, FString email, FString resetquestion, FString newpassword)
{
	UHTTPRequestHandler* httpHandler = UHTTPRequestHandler::MakeHTTPRequester();
	httpHandler->MakePasswordResetRequest("159.203.163.147/resetPassword’", username, email, resetquestion, newpassword);

	return httpHandler;
}
/// <summary>
/// Get all modules available on the server
/// </summary>
/// <returns></returns>
UHTTPRequestHandler* AMainMenuGameMode::GetModulesFromServer()
{
	UHTTPRequestHandler* httpHandler = UHTTPRequestHandler::MakeHTTPRequester();
	httpHandler->MakeModuleListRequest("159.203.163.147/langlist");

	return httpHandler;
}
/// <summary>
/// Get modules available from server, in an FModulePackage format that also states whether we are updating an downloaded one or downloading a new one
/// </summary>
/// <returns></returns>
TArray<FModulePackage> AMainMenuGameMode::GetServerModules()
{
	for (int x = 0; x < StaticGameSettings::servermodules.Num(); x++)
	{
		for (int y = 0; y < moduleHandler.Modules.Num(); y++)
		{
			if (moduleHandler.Modules[y].ModuleName.Equals(StaticGameSettings::servermodules[x].Module))
			{
				StaticGameSettings::servermodules[x].Status = moduleHandler.Modules[y].VersionNumber == StaticGameSettings::servermodules[x].Version ? 1 : 0;
			}
		}
	}
	return StaticGameSettings::servermodules;
}
/// <summary>
/// Get all puzzles recorded
/// </summary>
/// <returns></returns>
TArray<FPuzzleObject> AMainMenuGameMode::GetPuzzles()
{
	if (StaticGameSettings::runRecorderHandler != nullptr)
		return StaticGameSettings::runRecorderHandler->puzzles;
	else
	{
		TArray<FPuzzleObject> bob;
		return bob;
	}		
}
/// <summary>
/// Get current score
/// </summary>
/// <returns></returns>
int AMainMenuGameMode::GetScore()
{
	if (StaticGameSettings::runRecorderHandler != nullptr)
		return StaticGameSettings::runRecorderHandler->score;
	else
	{
		return -1;
	}
}
/// <summary>
/// Load all the modules, shallowload only loads module name and version number
/// </summary>
/// <param name="shallowload"></param>
void AMainMenuGameMode::LoadModules(bool shallowload)
{
	moduleHandler.LoadModules(shallowload);
}
/// <summary>
/// Download a language modules by name
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
UHTTPRequestHandler* AMainMenuGameMode::DownloadModule(const FString str)
{
	UHTTPRequestHandler* httpHandler = UHTTPRequestHandler::MakeHTTPRequester();
	httpHandler->DownloadModule("159.203.163.147/download/" + str, str);

	return httpHandler;
}
/// <summary>
/// Set The music volume
/// </summary>
/// <param name="val"></param>
void AMainMenuGameMode::SetMusicVolume(float val)
{
	StaticGameSettings::musicVolume = val;
}
/// <summary>
/// Set the puzzle volume
/// </summary>
/// <param name="val"></param>
void AMainMenuGameMode::SetPuzzleVolume(float val)
{
	StaticGameSettings::puzzleVolume = val;
}
/// <summary>
/// Set the sound effects volume
/// </summary>
/// <param name="val"></param>
void AMainMenuGameMode::SetSFXVolume(float val)
{
	StaticGameSettings::sfxVolume = val;
}
/// <summary>
/// (DEPRICATED)
/// </summary>
/// <param name="val"></param>
void AMainMenuGameMode::SetBrightnessValue(float val)
{
	StaticGameSettings::brightnessValue = val;
}
/// <summary>
/// Get the current music volume
/// </summary>
/// <returns></returns>
float AMainMenuGameMode::GetMusicVolume()
{
	return StaticGameSettings::musicVolume;
}
/// <summary>
/// Get the current puzzle volume
/// </summary>
/// <returns></returns>
float AMainMenuGameMode::GetPuzzleVolume()
{
	return StaticGameSettings::puzzleVolume;
}
/// <summary>
/// get the current sound effects volume
/// </summary>
/// <returns></returns>
float AMainMenuGameMode::GetSFXVolume()
{
	return StaticGameSettings::sfxVolume;
}
/// <summary>
/// (DEPRICATED)
/// </summary>
/// <returns></returns>
float AMainMenuGameMode::GetBrightnessValue()
{
	return StaticGameSettings::brightnessValue;
}

/*DEPRICATED SCRIPT TO LOAD MANY LOGS INTO DB*/
/*
TArray<FString> asdf;
StaticGameSettings::GetAllFilesInDirectory(&asdf, *(FPaths::GameSavedDir()), true, "", "csv");

TArray<FString> names = { "georgma", "eric", "test", "tyler" };
TArray<FString> gametypes = { "VR", "TPO", "TPP" };
for (int x = 0; x < asdf.Num(); x++)
{
TArray<FString> affd;
FFileHelper::LoadANSITextFileToStrings(*asdf[x], NULL, affd);

TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

JsonObject->SetStringField("lang_name", "Portuguese");
JsonObject->SetNumberField("lang_version", 1);
JsonObject->SetNumberField("score", FMath::Rand() % 500);
JsonObject->SetStringField("game_type", gametypes[FMath::Rand() % gametypes.Num()]);

//TArray<TSharedPtr<FJsonValue>> trms;
FString str = "";
for (int y = 3; y < affd.Num(); y++)
{
str.Append(affd[y]);
}

JsonObject->SetStringField("puzzles_attempted", str);

FString aaaaaa;
TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&aaaaaa);
FJsonSerializer::Serialize(JsonObject.ToSharedRef(), writer);

UHTTPRequestHandler* httpHandler = UHTTPRequestHandler::MakeHTTPRequester();
httpHandler->MakeHTTPRequest("POST", "159.203.163.147/log/player/" + names[FMath::Rand() % names.Num()], aaaaaa);
}
*/