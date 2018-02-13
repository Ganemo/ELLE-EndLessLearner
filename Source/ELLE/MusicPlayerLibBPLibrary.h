// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Engine.h"
#include "ImageUtils.h"
#include "ImageWrapper.h"
#include "MusicPlayerLibBPLibrary.generated.h"

UENUM(BlueprintType)
enum class EMPLibImageFormats : uint8 
{
	JPG		UMETA(DisplayName = "JPG"),
	PNG		UMETA(DisplayName = "PNG"),
	BMP		UMETA(DisplayName = "BMP"),
	ICO		UMETA(DisplayName = "ICO"),
	EXR		UMETA(DisplayName = "EXR"),
	ICNS	UMETA(DisplayName = "ICNS")
};

/* 
*	Function library class.
*	Each function in it is expected to be static and represents blueprint node that can be called in any blueprint.
*
*	When declaring function you can define metadata for the node. Key function specifiers will be BlueprintPure and BlueprintCallable.
*	BlueprintPure - means the function does not affect the owning object in any way and thus creates a node without Exec pins.
*	BlueprintCallable - makes a function which can be executed in Blueprints - Thus it has Exec pins.
*	DisplayName - full name of the node, shown when you mouse over the node and in the blueprint drop down menu.
*				Its lets you name the node using characters not allowed in C++ function names.
*	CompactNodeTitle - the word(s) that appear on the node.
*	Keywords -	the list of keywords that helps you to find node when you search for it using Blueprint drop-down menu. 
*				Good example is "Print String" node which you can find also by using keyword "log".
*	Category -	the category your node will be under in the Blueprint drop-down menu.
*
*	For more info on custom blueprint nodes visit documentation:
*	https://wiki.unrealengine.com/Custom_Blueprint_Node_Creation
*/
UCLASS()
class UMusicPlayerLibBPLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	template <class FunctorType>
	class PlatformFileFunctor : public IPlatformFile::FDirectoryVisitor	//GenericPlatformFile.h
	{
	public:

		virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
		{
			return Functor(FilenameOrDirectory, bIsDirectory);
		}

		PlatformFileFunctor(FunctorType&& FunctorInstance)
			: Functor(MoveTemp(FunctorInstance))
		{
		}

	private:
		FunctorType Functor;
	};

	template <class Functor>
	static PlatformFileFunctor<Functor> MakeDirectoryVisitor(Functor&& FunctorInstance)
	{
		return PlatformFileFunctor<Functor>(MoveTemp(FunctorInstance));
	}

	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//  Victory Get Directories
	//      Optional Search SubString!!!  by Rama
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	//Get Directories
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetDirectories", Keywords = "MusicPlayerLib file files"), Category = "MusicPlayerLib|File IO")
	static bool GetDirectories(const FString& FullPathOfBaseDir, TArray<FString>& DirsOut, bool Recursive = false, const FString& ContainsStr = "");	

	/** Obtain all files in a provided directory, with optional extension filter. All files are returned if Ext is left blank. Returns false if operation could not occur. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetFiles", Keywords = "MusicPlayerLib file files"), Category = "MusicPlayerLib|File IO")
	static bool GetFiles(TArray<FString>& Files, FString RootFolderFullPath, FString Ext);

	/** Obtain all files in a provided root directory, including all subdirectories, with optional extension filter. All files are returned if Ext is left blank. The full file path is returned because the file could be in any subdirectory. Returns false if operation could not occur. */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetFilesInRootAndAllSubFolders", Keywords = "MusicPlayerLib file files"), Category = "MusicPlayerLib|File IO")
	static bool GetFilesInRootAndAllSubFolders(TArray<FString>& Files, FString RootFolderFullPath, FString Ext);

	/** InstallDir/GameName */
	UFUNCTION(BlueprintPure, meta = (DisplayName = "GetGameRootDirectory", Keywords = "MusicPlayerLib Game Root Directory"), Category = "MusicPlayerLib|File IO")
	static FString GetGameRootDirectory();

	/**
	*
	* Contributed by: Mindfane
	*
	* Split a string into an array of substrings based on the given delimitter.
	* Unlike ParseIntoArray() function which expects single character delimitters, this function can accept a delimitter that is also a string.
	*
	* @param InputString - The string that is to be exploded.
	* @param Separator - The delimitter that is used for splitting (multi character strings are allowed)
	* @param limit - If greater than zero, returns only the first x strings. Otherwsie returns all the substrings
	* @param bTrimElelements - If True, then each subsctring is processed and any leading or trailing whitespcaes are trimmed.
	*/
	UFUNCTION(BlueprintPure, meta = (DisplayName = "ExplodeString", Keywords = "MusicPlayerLib string"), Category = "MusicPlayerLib|File IO")
	static void ExplodeString(TArray<FString>& OutputStrings, FString InputString, FString Separator = ",", int32 Limit = 0, bool bTrimElements = false);

	/** Load a Texture2D from a JPG,PNG,BMP,ICO,EXR,ICNS file! IsValid tells you if file path was valid or not. Enjoy! -Rama */
	UFUNCTION(BlueprintPure, meta = (Keywords = "image png jpg jpeg bmp bitmap ico icon exr icns"), Category = "MusicPlayerLib|File IO" )
	static UTexture2D* LoadTexture2D_FromFile(const FString& FullFilePath, EMPLibImageFormats ImageFormat, bool& IsValid, int32& Width, int32& Height);


	/** Contributed by UE4 forum member n00854180t! Plays a .ogg sound from file, attached to and following the specified component. This is a fire and forget sound. Replication is also not handled at this point.
	* @param FilePath - Path to sound file to play
	* @param AttachComponent - Component to attach to.
	* @param AttachPointName - Optional named point within the AttachComponent to play the sound at
	* @param Location - Depending on the value of Location Type this is either a relative offset from the attach component/point or an absolute world position that will be translated to a relative offset
	* @param LocationType - Specifies whether Location is a relative offset or an absolute world position
	* @param bStopWhenAttachedToDestroyed - Specifies whether the sound should stop playing when the owner of the attach to component is destroyed.
	* @param VolumeMultiplier - Volume multiplier
	* @param PitchMultiplier - PitchMultiplier
	* @param AttenuationSettings - Override attenuation settings package to play sound with
	*/
	UFUNCTION(BlueprintCallable, meta = (VolumeMultiplier = "1.0", PitchMultiplier = "1.0", AdvancedDisplay = "2", UnsafeDuringActorConstruction = "true"), Category = "MusicPlayerLib|Audio")
	static class UAudioComponent* PlaySoundAttachedFromFile(const FString& FilePath, class USceneComponent* AttachToComponent, FName AttachPointName = NAME_None, FVector Location = FVector(ForceInit), EAttachLocation::Type LocationType = EAttachLocation::SnapToTarget, bool bStopWhenAttachedToDestroyed = false, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, class USoundAttenuation* AttenuationSettings = NULL);

	/** Contributed by UE4 forum member n00854180t! Plays a .ogg sound at the given location. This is a fire and forget sound and does not travel with any actor. Replication is also not handled at this point.
	*
	* NOT SUPPORTED ON PS4.
	*
	* @param FilePath - Path to sound file to play
	* @param Location - World position to play sound at
	* @param World - The World in which the sound is to be played
	* @param VolumeMultiplier - Volume multiplier
	* @param PitchMultiplier - PitchMultiplier
	* @param AttenuationSettings - Override attenuation settings package to play sound with
	*/
	UFUNCTION(BlueprintCallable, meta = (HidePin = "WorldContextObject", DefaultToSelf = "WorldContextObject", VolumeMultiplier = "1.0", PitchMultiplier = "1.0", AdvancedDisplay = "3", UnsafeDuringActorConstruction = "true"), Category = "MusicPlayerLib|Audio")
	static void PlaySoundAtLocationFromFile(UObject* WorldContextObject, const FString& FilePath, FVector Location, float VolumeMultiplier = 1.f, float PitchMultiplier = 1.f, float StartTime = 0.f, class USoundAttenuation* AttenuationSettings = NULL);

	/** Contributed by UE4 forum member n00854180t! Creates a USoundWave* from file path.
	* Read .ogg header file and refresh USoundWave metadata.
	*
	* NOT SUPPORTED ON PS4.
	*
	* @param FilePath		path to file to create sound wave from
	*/
	UFUNCTION(BlueprintCallable, meta = (DisplayName = "GetSoundWaveFromFile", Keywords = "MusicPlayerLib Sond Wave File"), Category = "MusicPlayerLib|Audio")
	static class USoundWave* GetSoundWaveFromFile(const FString& FilePath);



#if !PLATFORM_PS4
private:
	// Thanks to @keru for the base code for loading an Ogg into a USoundWave: 
	// https://forums.unrealengine.com/showthread.php?7936-Custom-Music-Player&p=97659&viewfull=1#post97659

	/**
	* Read .ogg header file and refresh USoundWave metadata. NOT SUPPORTED BY PS4
	* @param sw             wave to put metadata
	* @param rawFile        pointer to src file in memory
	* @return 0     if everything is ok
	*                 1 if couldn't read metadata.
	*/
	static int32 fillSoundWaveInfo(USoundWave* sw, TArray<uint8>* rawFile);



	/**
	* Tries to find out FSoundSource object associated to the USoundWave. NOT SUPPORTED BY PS4
	* @param sw     wave, search key
	* @return 0 if wave found and correctly set
	*        -1 if error: sound device not set
	*        -2 if error: sound wave not found
	*/
	//static int32 findSource(class USoundWave* sw, class FSoundSource* out_audioSource);
#endif //PLATFORM_PS4

};
