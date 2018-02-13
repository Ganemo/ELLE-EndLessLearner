// Fill out your copyright notice in the Description page of Project Settings.

#include "ELLE.h"
#include "HTTPRequestHandler.h"
#include "MainMenuGameMode.h"
#include "ZipFileFunctionLibrary.h"

UHTTPRequestHandler::UHTTPRequestHandler():
	FileUrl(TEXT(""))
	, FileSavePath(TEXT(""))
{
}

UHTTPRequestHandler::~UHTTPRequestHandler()
{
}

/// <summary>
/// Creates an HTTPRequestHandler object
/// </summary>
/// <returns>UHTTPRequestHandler*</returns>
UHTTPRequestHandler* UHTTPRequestHandler::MakeHTTPRequester()
{
	UHTTPRequestHandler* Downloader = NewObject<UHTTPRequestHandler>();
	return Downloader;
}

/// <summary>
/// Determines if an HTTP request is valid
/// </summary>
/// <param name="type">the HTTP request type</param>
/// <returns>bool</returns>
bool IsValidHTTPRequestType(FString type)
{
	if (type.Equals("GET")
		|| type.Equals("HEAD")
		|| type.Equals("POST")
		|| type.Equals("PUT")
		|| type.Equals("PUT")
		|| type.Equals("DELETE")
		|| type.Equals("CONNECT")
		|| type.Equals("OPTIONS")
		|| type.Equals("TRACE")
		|| type.Equals("PATCH"))
		return true;
	else
		return false;
}

/// <summary>
/// Inline function used to create a directory, or verify if that directory exists
/// </summary>
/// <param name="TestDir">the absolute path of the directory</param>
/// <returns>bool</returns>
FORCEINLINE bool VerifyOrCreateDirectory(const FString& TestDir)
{
	IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

	// Directory Exists?
	if (!PlatformFile.DirectoryExists(*TestDir))
	{
		PlatformFile.CreateDirectory(*TestDir);

		if (!PlatformFile.DirectoryExists(*TestDir))
		{
			return false;
		}
	}
	return true;
}

/// <summary>
/// Call to download a language module from the given URL and it's destination location
/// </summary>
/// <param name="Url">HTTP request URL</param>
/// <param name="FileName">Download location</param>
/// <returns>UHTTPRequestHandler*</returns>
UHTTPRequestHandler* UHTTPRequestHandler::DownloadModule(const FString& Url, FString FileName)
{
	FileUrl = Url.Replace(TEXT(" "), TEXT("%20"));;
	
	VerifyOrCreateDirectory(FPaths::GameSavedDir() + "LanguageModules/" + FileName);
	FileSavePath = FPaths::GameSavedDir() + "LanguageModules/" + FileName + "/" + FileName + ".zip";

	TSharedRef< IHttpRequest > HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("GET");
	HttpRequest->SetURL(FileUrl);
	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHTTPRequestHandler::OnReady_File);
	HttpRequest->OnRequestProgress().BindUObject(this, &UHTTPRequestHandler::OnProgress_Internal);

	// Execute the request
	HttpRequest->ProcessRequest();
	AddToRoot();

	return this;
}

/// <summary>
/// Perform some HTTP request
/// </summary>
/// <param name="type">Type of HTTP request (GET, POST, etc)</param>
/// <param name="Url">Destination URL</param>
/// <param name="content">Content for the request</param>
/// <returns>UHTTPRequestHandler*</returns>
UHTTPRequestHandler* UHTTPRequestHandler::MakeHTTPRequest(const FString& type, const FString& Url, const FString& content)
{
	if (IsValidHTTPRequestType(type))
	{
		TSharedRef< IHttpRequest > HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetVerb(type);
		HttpRequest->SetURL(Url);

		if (StaticGameSettings::isLoggedIn == 1)
			HttpRequest->SetHeader("Authorization", "JWT " + StaticGameSettings::JWTToken);

		if (content.Len() > 0)
		{
			HttpRequest->SetHeader("Content-Type", "application/json");
			HttpRequest->SetContentAsString(content);
		}
		
		HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHTTPRequestHandler::OnReady_JSON);
		HttpRequest->OnRequestProgress().BindUObject(this, &UHTTPRequestHandler::OnProgress_Internal);

		// Execute the request
		HttpRequest->ProcessRequest();
		AddToRoot();
	}

	return this;
}

/// <summary>
/// Retrieve the list of available modules
/// </summary>
/// <param name="Url">Destination URL</param>
/// <returns>UHTTPRequestHandler*</returns>
UHTTPRequestHandler* UHTTPRequestHandler::MakeModuleListRequest(const FString& Url)
{
	TSharedRef< IHttpRequest > HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("GET");
	HttpRequest->SetURL(Url);

	HttpRequest->SetHeader("Authorization", "JWT " + StaticGameSettings::JWTToken);

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHTTPRequestHandler::OnReady_Modules);
	HttpRequest->OnRequestProgress().BindUObject(this, &UHTTPRequestHandler::OnProgress_Internal);

	// Execute the request
	HttpRequest->ProcessRequest();
	AddToRoot();

	return this;
}

/// <summary>
/// Attempt To Log in
/// </summary>
/// <param name="Url">Destination URL</param>
/// <param name="username"></param>
/// <param name="pw"></param>
/// <returns>UHTTPRequestHandler*</returns>
UHTTPRequestHandler* UHTTPRequestHandler::MakeLogInRequest(const FString& Url, const FString& username, const FString& pw)
{
	TSharedRef< IHttpRequest > HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);

	HttpRequest->SetHeader("Content-Type", "application/json");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField("username", username);
	JsonObject->SetStringField("password", pw);

	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpRequest->SetContentAsString(OutputString);
	

	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHTTPRequestHandler::OnReady_Login);
	HttpRequest->OnRequestProgress().BindUObject(this, &UHTTPRequestHandler::OnProgress_Internal);

	// Execute the request
	HttpRequest->ProcessRequest();
	AddToRoot();

	return this;
}

/// <summary>
/// Create a new account
/// </summary>
/// <param name="Url">Destination URL</param>
/// <param name="username"></param>
/// <param name="pw"></param>
/// <param name="email"></param>
/// <param name="sex"></param>
/// <param name="age"></param>
/// <param name="security"></param>
/// <param name="motivation"></param>
/// <returns>UHTTPRequestHandler*</returns>
UHTTPRequestHandler* UHTTPRequestHandler::MakeAccountCreationRequest(const FString& Url, const FString& username, const FString& pw, const FString& email, const FString& sex, const int age, const FString& security, const FString& motivation)
{
	TSharedRef< IHttpRequest > HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("POST");
	HttpRequest->SetURL(Url);

	HttpRequest->SetHeader("Content-Type", "application/json");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField("username", username);
	JsonObject->SetStringField("password", pw);
	JsonObject->SetStringField("email", email);
	JsonObject->SetStringField("isAdmin", "0");
	JsonObject->SetStringField("sex", sex);
	JsonObject->SetNumberField("age", age);
	JsonObject->SetStringField("password_reset", security);
	JsonObject->SetStringField("motivation", sex);

	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpRequest->SetContentAsString(OutputString);


	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHTTPRequestHandler::OnReady_JSON);
	HttpRequest->OnRequestProgress().BindUObject(this, &UHTTPRequestHandler::OnProgress_Internal);

	// Execute the request
	HttpRequest->ProcessRequest();
	AddToRoot();

	return this;
}

/// <summary>
/// Attempt to reset a password
/// </summary>
/// <param name="Url">Destination URL</param>
/// <param name="username"></param>
/// <param name="email"></param>
/// <param name="resetquestion"></param>
/// <param name="newpassword"></param>
/// <returns>UHTTPRequestHandler*</returns>
UHTTPRequestHandler* UHTTPRequestHandler::MakePasswordResetRequest(const FString& Url, const FString& username, const FString& email, const FString& resetquestion, const FString& newpassword)
{
	TSharedRef< IHttpRequest > HttpRequest = FHttpModule::Get().CreateRequest();
	HttpRequest->SetVerb("PUT");
	HttpRequest->SetURL(Url);

	HttpRequest->SetHeader("Content-Type", "application/json");

	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject);

	JsonObject->SetStringField("username", username);
	JsonObject->SetStringField("password_reset", resetquestion);
	JsonObject->SetStringField("email", email);
	JsonObject->SetStringField("new_password", newpassword);

	FString OutputString;
	TSharedRef< TJsonWriter<> > Writer = TJsonWriterFactory<>::Create(&OutputString);
	FJsonSerializer::Serialize(JsonObject.ToSharedRef(), Writer);

	HttpRequest->SetContentAsString(OutputString);


	HttpRequest->OnProcessRequestComplete().BindUObject(this, &UHTTPRequestHandler::OnReady_JSON);
	HttpRequest->OnRequestProgress().BindUObject(this, &UHTTPRequestHandler::OnProgress_Internal);

	// Execute the request
	HttpRequest->ProcessRequest();
	AddToRoot();

	return this;
}

/// <summary>
/// Async Function called when a file is finished downloading
/// </summary>
/// <param name="Request">The request sent</param>
/// <param name="Response">The response</param>
/// <param name="bWasSuccessful">If the download was successful</param>
void UHTTPRequestHandler::OnReady_File(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	RemoveFromRoot();
	Request->OnProcessRequestComplete().Unbind();

	if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		// SAVE FILE
		IPlatformFile& PlatformFile = FPlatformFileManager::Get().GetPlatformFile();

		// create save directory if not existent
		FString Path, Filename, Extension;
		FPaths::Split(FileSavePath, Path, Filename, Extension);

		//delete old module if it exists
		if (PlatformFile.FileExists(*FileSavePath))
		{
			PlatformFile.DeleteFile(*FileSavePath);
		}

		//create save directory
		if (!PlatformFile.DirectoryExists(*Path))
		{
			if(!PlatformFile.CreateDirectoryTree(*Path))
			{
				OnResult.Broadcast(EDownloadResult::DirectoryCreationFailed, "Failed To Create Directory");
				return;
			}
		}
		else
		{
			PlatformFile.DeleteDirectory(*Path);

			if (!PlatformFile.CreateDirectoryTree(*Path))
			{
				OnResult.Broadcast(EDownloadResult::DirectoryCreationFailed, "Failed To Create Directory");
				return;
			}
		}

		// open/create the file
		IFileHandle* FileHandle = PlatformFile.OpenWrite(*FileSavePath);
		if (FileHandle)
		{
			// write the file
			FileHandle->Write(Response->GetContent().GetData(), Response->GetContentLength());
			// Close the file
			delete FileHandle;

			UZipFileFunctionLibrary::UnzipTo(FileSavePath, Path, this);
		}
		else
		{
			OnResult.Broadcast(EDownloadResult::SaveFailed, FileSavePath);
		}
	}
	else
	{
		OnResult.Broadcast(EDownloadResult::DownloadFailed, "DownloadFailed");
	}
}

/// <summary>
/// Async function called when a HTTP request returns just a JSON response
/// </summary>
/// <param name="Request">The request sent</param>
/// <param name="Response">The response</param>
/// <param name="bWasSuccessful">If the request was successful</param>
void UHTTPRequestHandler::OnReady_JSON(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	RemoveFromRoot();
	Request->OnProcessRequestComplete().Unbind();


	UE_LOG(LogTemp, Log, TEXT("%s"), *Response->GetContentAsString());

	if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		OnResult.Broadcast(EDownloadResult::Success, Response->GetContentAsString());

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			
		}
	}
	else
	{
		OnResult.Broadcast(EDownloadResult::RequestFailed, "");
	}
}

/// <summary>
/// Async function called when modules are done downloading
/// </summary>
/// <param name="Request">The request sent</param>
/// <param name="Response">The response</param>
/// <param name="bWasSuccessful">If the Request is successful</param>
void UHTTPRequestHandler::OnReady_Modules(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	RemoveFromRoot();
	Request->OnProcessRequestComplete().Unbind();

	UE_LOG(LogTemp, Log, TEXT("%s"), *Response->GetContentAsString());

	if (Response.IsValid() && EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{
		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());

		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			StaticGameSettings::servermodules.Empty();

			TArray<TSharedPtr<FJsonValue>> mods = JsonObject->GetArrayField("modules");
			for (int x = 0; x < mods.Num(); x++)
			{
				TSharedPtr<FJsonObject> obj = mods[x]->AsObject();
				

				FModulePackage modul;
				modul.Module = obj->GetStringField("lang_name");
				modul.Version = obj->GetIntegerField("version");
				modul.Status = -1;

				StaticGameSettings::servermodules.Add(modul);
			}
		}

		OnResult.Broadcast(EDownloadResult::Success, Response->GetContentAsString());
	}
	else
	{
		OnResult.Broadcast(EDownloadResult::RequestFailed, "");
	}
}

/// <summary>
/// Async function called when login completes
/// </summary>
/// <param name="Request">The request sent</param>
/// <param name="Response">The response</param>
/// <param name="bWasSuccessful"></param>
void UHTTPRequestHandler::OnReady_Login(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	RemoveFromRoot();
	Request->OnProcessRequestComplete().Unbind();

	if (!Response.IsValid())
	{
		OnResult.Broadcast(EDownloadResult::RequestFailed, "");
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("%s"), *Response->GetContentAsString());

	if (EHttpResponseCodes::IsOk(Response->GetResponseCode()))
	{

		TSharedPtr<FJsonObject> JsonObject;
		TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());


		if (FJsonSerializer::Deserialize(Reader, JsonObject))
		{
			StaticGameSettings::JWTToken = JsonObject.Get()->GetStringField("access_token");
			if (StaticGameSettings::JWTToken.Len() > 0)
			{
				StaticGameSettings::isLoggedIn = true;
				OnResult.Broadcast(EDownloadResult::Success, "");
			}
			else
			{
				StaticGameSettings::isLoggedIn = false;
				OnResult.Broadcast(EDownloadResult::RequestFailed, "");
			}
		}
	}
	else
	{
		StaticGameSettings::isLoggedIn = false;
		OnResult.Broadcast(EDownloadResult::RequestFailed, "");
	}
}

/// <summary>
/// Async function called when any HTTP request is recieving chunks
/// </summary>
/// <param name="Request">The request sent</param>
/// <param name="BytesSent">How many bytes have been sent</param>
/// <param name="BytesReceived">How many bytes have been recieved</param>
void UHTTPRequestHandler::OnProgress_Internal(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived)
{
	int32 FullSize = Request->GetContentLength();
	OnHTTPProgress.Broadcast(BytesSent, BytesReceived, FullSize);
}

/// <summary>
/// Unzip async progress function
/// </summary>
/// <param name="archive">destination url</param>
/// <param name="percentage">how much is done</param>
/// <param name="bytes">total bytes unzipped</param>
void UHTTPRequestHandler::OnProgress_Implementation(const FString& archive, float percentage, int32 bytes)
{

}

/// <summary>
/// When unzip is complete
/// </summary>
/// <param name="archive">Destination directory</param>
/// <param name="CompletionState">What the final state was</param>
void UHTTPRequestHandler::OnDone_Implementation(const FString& archive, EZipUtilityCompletionState CompletionState)
{
	FString Path, Filename, Extension;
	FPaths::Split(FileSavePath, Path, Filename, Extension);

	TArray<FString> files;
	StaticGameSettings::GetAllFilesInDirectory(&files, *(Path), true, "", "wav");
	
	for (int x = 0; x < files.Num(); x++)
	{
		FString Path1, Filename1, Extension1;
		FPaths::Split(files[x], Path1, Filename1, Extension1);

		FString s = " -o " + (Path1 + "/" + Filename1) + ".ogg " + (Path1 + "/" + Filename1) + ".wav";

		int output;
		FString s1;
		FString s2;
		FPlatformProcess::ExecProcess(*(FPaths::GameSavedDir() + "LanguageModules/oggenc2.exe"), *(s), &output, &s1, &s2);
	}

	OnResult.Broadcast(EDownloadResult::Success, FileSavePath);
}

/// <summary>
/// Async unzip function called when unzip is started
/// </summary>
/// <param name="archive">Destination</param>
/// <param name="bytes">total bytes</param>
void UHTTPRequestHandler::OnStartProcess_Implementation(const FString& archive, int32 bytes)
{

}

/// <summary>
/// Async unzip function called when a single file unzip is complete
/// </summary>
/// <param name="archive">Destination</param>
/// <param name="file">File Destination path</param>
void UHTTPRequestHandler::OnFileDone_Implementation(const FString& archive, const FString& file)
{

}

/// <summary>
/// Async unzip function called when 
/// </summary>
/// <param name="archive">Destinatino</param>
/// <param name="file">File looking for</param>
/// <param name="size">Total size</param>
void UHTTPRequestHandler::OnFileFound_Implementation(const FString& archive, const FString& file, int32 size)
{

}