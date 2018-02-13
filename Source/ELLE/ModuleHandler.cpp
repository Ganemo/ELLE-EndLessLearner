
#include "ELLE.h"
#include "Json.h"
#include "ModuleHandler.h"
#include "HTTPRequestHandler.h"

ModuleHandler::ModuleHandler()
{
}

ModuleHandler::~ModuleHandler()
{
}

/// <summary>
/// Load all language modules manifests (JSON) into memory
/// </summary>
/// <param name="isShallow"></param>
void ModuleHandler::LoadModules(bool isShallow)
{
	Modules.Empty();

	TArray<FString> files;
	StaticGameSettings::GetAllFilesInDirectory(&files, *(FPaths::GameSavedDir() + "LanguageModules/"), true, "", "json");

	for (int32 i = 0; i < files.Num(); ++i)
	{
		FString jsonInfo;
		FFileHelper::LoadFileToString(jsonInfo, *files[i]);

		FLanguageModule langMod;

		if(isShallow)
			ParseJSONModule_Shallow(&langMod, jsonInfo);
		else
			ParseJSONModule(&langMod, jsonInfo);

		Modules.Add(langMod);
	}
}

/// <summary>
/// Get a random language module
/// </summary>
/// <returns></returns>
FLanguageModule* ModuleHandler::GetRandomModule()
{
	return &Modules[rand() % Modules.Num()];
}

/// <summary>
/// Parse a JSON module manifest and only load name a version
/// </summary>
/// <param name="langMod"></param>
/// <param name="rawJson"></param>
void ModuleHandler::ParseJSONModule_Shallow(FLanguageModule* langMod, FString rawJson)
{
	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(rawJson);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		langMod->ModuleName = JsonParsed->GetStringField("language");
		langMod->VersionNumber = JsonParsed->GetIntegerField("version");
	}
}
/// <summary>
/// Parse a JSON module manifest and load all info
/// </summary>
/// <param name="langMod"></param>
/// <param name="rawJson"></param>
void ModuleHandler::ParseJSONModule(FLanguageModule* langMod, FString rawJson)
{
	TSharedPtr<FJsonObject> JsonParsed;
	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(rawJson);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		langMod->ModuleName = JsonParsed->GetStringField("language");
		langMod->VersionNumber = JsonParsed->GetIntegerField("version");
		TArray<TSharedPtr<FJsonValue>> trms = JsonParsed->GetArrayField("terms");
		for (int x = 0; x < trms.Num(); x++)
		{
			FTerm* term = new FTerm();

			TSharedPtr<FJsonObject> obj = trms[x].ToSharedRef()->AsObject();

			term->Term = obj->GetStringField("term");
			term->TermInEnglish = obj->GetStringField("translation");
			term->AudioPath = obj->GetStringField("audio_term");
			term->TranslatedAudioPath = obj->GetStringField("audio_translation");
			term->ImagePath = obj->GetStringField("image");

			langMod->Terms.Add(*term);
		}
	}
}