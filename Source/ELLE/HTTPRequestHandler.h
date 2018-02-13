// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Http.h"
#include "Json.h"
#include "ZipUtilityInterface.h"
#include "HTTPRequestHandler.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnResult, const EDownloadResult, Result, const FString, Response);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnProgress, const int32, BytesSent, const int32, BytesReceived, const int32, ContentLength);

/**
 * Download a file from a URL and save it locally.
 */
UCLASS(BlueprintType, Category = "HTTP")
class ELLE_API UHTTPRequestHandler : public UObject, public IZipUtilityInterface
{
	GENERATED_BODY()

public:
	/**
	* Bind to know when the download is complete (even if it fails).
	*/
	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FOnResult OnResult;
	/**
	* Bind to know when the download is complete (even if it fails).
	*/
	UPROPERTY(BlueprintAssignable, Category = "HTTP")
	FOnProgress OnHTTPProgress;

	/**
	* The URL used to start this download.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "HTTP")
	FString FileUrl;
	/**
	* The path set to save the downloaded file.
	*/
	UPROPERTY(BlueprintReadOnly, Category = "HTTP")
	FString FileSavePath;


	UHTTPRequestHandler();
	~UHTTPRequestHandler();

	/**
	* Instantiates a FileDownloader object, starts downloading and saves it when done.
	* 
	* @return The FileDownloader object. Bind to it's OnResult event to know when it's done downloading.
	*/
	UFUNCTION(BlueprintCallable, Meta = (DisplayName = "Create Downloader"), Category = "HTTP")
	static UHTTPRequestHandler* MakeHTTPRequester();

	/**
	* Starts downloading a file and saves it when done. Bind to the OnResult
	* event to know when the download is done (preferrably, before calling this function).
	* 
	* @param Url		The file Url to be downloaded.
	* @param SavePath	The absolute path and file name to save the downloaded file.
	* @return Returns itself.
	*/
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	UHTTPRequestHandler* DownloadModule(const FString& Url, FString FileName);
	
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	UHTTPRequestHandler* MakeHTTPRequest(const FString& type, const FString& Url, const FString& content);

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	UHTTPRequestHandler* MakeModuleListRequest(const FString& Url);

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	UHTTPRequestHandler* MakeLogInRequest(const FString& Url, const FString& username, const FString& pw);
	
	UFUNCTION(BlueprintCallable, Category = "HTTP")
	UHTTPRequestHandler* MakeAccountCreationRequest(const FString& Url, const FString& username, const FString& pw, const FString& email, const FString& sex, const int age, const  FString& security, const FString& motivation);

	UFUNCTION(BlueprintCallable, Category = "HTTP")
	UHTTPRequestHandler* MakePasswordResetRequest(const FString& Url, const FString& username, const FString& email, const FString& resetquestion, const FString& newpassword);

	/*
	* Zip callback functions
	*/
	virtual void OnProgress_Implementation(const FString& archive, float percentage, int32 bytes) override;
	virtual void OnDone_Implementation(const FString& archive, EZipUtilityCompletionState CompletionState) override;
	virtual void OnStartProcess_Implementation(const FString& archive, int32 bytes) override;
	virtual void OnFileDone_Implementation(const FString& archive, const FString& file) override;
	virtual void OnFileFound_Implementation(const FString& archive, const FString& file, int32 size) override;

private:

	/*
	* HTTP callback functions
	*/
	void OnReady_File(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnReady_JSON(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnReady_Modules(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnReady_Login(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
	void OnProgress_Internal(FHttpRequestPtr Request, int32 BytesSent, int32 BytesReceived);


};
