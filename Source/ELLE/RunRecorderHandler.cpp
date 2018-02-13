// Fill out your copyright notice in the Description page of Project Settings.

#include "ELLE.h"
#include "RunRecorderHandler.h"
#include "HttpRequestHandler.h"
#include "StaticGameSettings.h"
#include "Json.h"

RunRecorderHandler::RunRecorderHandler()
{
}

RunRecorderHandler::~RunRecorderHandler()
{
	CloseRunFile();
}

/// <summary>
/// Open a run file for recording
/// </summary>
/// <param name="fileName"></param>
/// <param name="gameType"></param>
/// <param name="difficulty"></param>
/// <param name="language"></param>
void RunRecorderHandler::OpenRunFile(FString fileName, FString gameType, FString difficulty, FString language)
{
	//open the file to print to
	//runFile = new ofstream();

	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();
	//make sure the player logs directory exists
	if (!PlatformFile.DirectoryExists(*(FPaths::GameDir() + "PlayerLogs/")))
	{
		if (!PlatformFile.CreateDirectoryTree(*(FPaths::GameDir() + "PlayerLogs/")))
		{
			return;
		}
	}

	//remember the path
	path = FPaths::GameDir() + "PlayerLogs/" + fileName + ".csv";
	//open the log file
	//runFile->open(*path);
	fileOpen = true;

	//add preliminary information
	FString str = "Name:," + StaticGameSettings::userName + "\n"
		+ "DateTime:," + FDateTime::Now().ToString() + "\n"
		+ "GameType:," + gameType + "\n"
		+ "Difficulty:," + difficulty + "\n"
		+ "Language:," + language + "\n";
	//runFile->write(TCHAR_TO_ANSI(*(str)), str.Len());
	FFileHelper::SaveStringToFile(str, *path, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), 0x08);

	FString str2 = "PuzzleType,Correct,Answer,Options\n";
	//runFile->write(TCHAR_TO_ANSI(*(str2)), str2.Len());
	FFileHelper::SaveStringToFile(str2, *path, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), 0x08);

	UE_LOG(LogTemp, Log, TEXT("File %s open"), *(path));
}
/// <summary>
/// record a puzzle in line form
/// </summary>
/// <param name="line"></param>
void RunRecorderHandler::RecordLine(FString line)
{
	FFileHelper::SaveStringToFile(line, *path, FFileHelper::EEncodingOptions::AutoDetect, &IFileManager::Get(), 0x08);

	//runFile->write(TCHAR_TO_ANSI(*line), line.Len());

	UE_LOG(LogTemp, Log, TEXT("Recorded %s"), *(line));
}
/// <summary>
/// Close a run file
/// </summary>
void RunRecorderHandler::CloseRunFile()
{

	fileOpen = false;

	if (StaticGameSettings::isLoggedIn)
	{
		TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

		JsonObject->SetStringField("lang_name", StaticGameSettings::selectedLanguage.ModuleName);
		JsonObject->SetNumberField("lang_version", StaticGameSettings::selectedLanguage.VersionNumber);

		FString gameType = "";
		switch (StaticGameSettings::gameType)
		{
		case EGameType::FirstPerson_VR: gameType = "VR";
			break;
		case EGameType::ThirdPerson_OverTheShoulder: gameType = "TPO";
			break;
		case EGameType::ThirdPerson_Perpendicular: gameType = "TPP";
			break;
		}
		JsonObject->SetStringField("game_type", gameType);


		JsonObject->SetNumberField("score", StaticGameSettings::runRecorderHandler->score);

		FString line = "PuzzleType,Correct,Answer,Options\n";
		for (int x = 0; x < puzzles.Num(); x++)
		{
			FString typestring;
			switch (puzzles[x].Type)
			{
			case Text_Image: typestring = "T-I";
				break;
			case Audio_Image: typestring = "A-I";
				break;
			case Image_Text: typestring = "I-T";
				break;
			case Text_Text: typestring = "T-T";
				break;
			case Audio_Text: typestring = "A-T";
				break;
			default: typestring = "ERROR";
				break;
			}

			line += typestring + "," + (puzzles[x].CorrectlySolved ? "T" : "F") + "," + puzzles[x].Focus;
			for (int y = 0; y < puzzles[x].Options.Num(); y++)
			{
				line += "," + puzzles[y].Options[y];
			}
			line += "\n";
		}

		JsonObject->SetStringField("puzzles_attempted", line);

		FString aaaaaa;
		TSharedRef<TJsonWriter<>> writer = TJsonWriterFactory<>::Create(&aaaaaa);
		FJsonSerializer::Serialize(JsonObject.ToSharedRef(), writer);

		UHTTPRequestHandler* httpHandler = UHTTPRequestHandler::MakeHTTPRequester();
		httpHandler->MakeHTTPRequest("POST", "159.203.163.147/log/player/" + StaticGameSettings::userName, aaaaaa);
	}

	UE_LOG(LogTemp, Log, TEXT("File closed"));
}

/// <summary>
/// Add a puzzle to the record
/// </summary>
/// <param name="correctlysolved"></param>
/// <param name="focus"></param>
/// <param name="options"></param>
/// <param name="type"></param>
void RunRecorderHandler::AddPuzzle(bool correctlysolved, FString focus, TArray<FString> options, EPuzzleType type)
{
	FPuzzleObject obj;

	obj.CorrectlySolved = correctlysolved;
	obj.Focus = focus;
	obj.Options = options;
	obj.Type = type;

	puzzles.Add(obj);

	FString line = "";
	FString typestring;
	switch (type)
	{
	case Text_Image: typestring = "T-I";
		break;
	case Audio_Image: typestring = "A-I";
		break;
	case Image_Text: typestring = "I-T";
		break;
	case Text_Text: typestring = "T-T";
		break;
	case Audio_Text: typestring = "A-T";
		break;
	default: typestring = "ERROR";
		break;
	}

	line += typestring + "," + (correctlysolved ? "T" : "F") + "," + focus;
	for (int y = 0; y < options.Num(); y++)
	{
		line += "," + options[y];
	}
	line += "\n";
	
	RecordLine(line);
}