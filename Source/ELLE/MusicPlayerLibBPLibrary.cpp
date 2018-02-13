// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "ELLE.h"
#include "MusicPlayerLibBPLibrary.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "Components/AudioComponent.h"
#include "AudioDecompress.h"
#include "AudioDevice.h"
#include "ActiveSound.h"
#include "Audio.h"
#include "Developer/TargetPlatform/Public/Interfaces/IAudioFormat.h"
#include "VorbisAudioInfo.h"


UMusicPlayerLibBPLibrary::UMusicPlayerLibBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{

}


bool UMusicPlayerLibBPLibrary::GetDirectories(const FString& FullPathOfBaseDir, TArray<FString>& DirsOut, bool Recursive, const FString& ContainsStr)
{
	FString Str;
	auto FilenamesVisitor = MakeDirectoryVisitor(
		[&](const TCHAR* FilenameOrDirectory, bool bIsDirectory)
	{
		if (bIsDirectory)
		{
			//Using a Contains Filter?
			if (ContainsStr != "")
			{
				Str = FPaths::GetCleanFilename(FilenameOrDirectory);

				//Only if Directory Contains Str
				if (Str.Contains(ContainsStr))
				{
					if (Recursive) DirsOut.Push(FilenameOrDirectory); //need whole path for recursive
					else DirsOut.Push(Str);
				}
			}

			//Get ALL Directories!
			else
			{
				//Just the Directory
				Str = FPaths::GetCleanFilename(FilenameOrDirectory);

				if (Recursive) DirsOut.Push(FilenameOrDirectory); //need whole path for recursive
				else DirsOut.Push(Str);
			}
		}
		return true;
	}
	);

	if (Recursive)
	{
		return FPlatformFileManager::Get().GetPlatformFile().IterateDirectoryRecursively(*FullPathOfBaseDir, FilenamesVisitor);
	}
	else
	{
		return FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*FullPathOfBaseDir, FilenamesVisitor);
	}
}


bool UMusicPlayerLibBPLibrary::GetFiles(TArray<FString>& Files, FString RootFolderFullPath, FString Ext)
{
	if (RootFolderFullPath.Len() < 1) return false;

	FPaths::NormalizeDirectoryName(RootFolderFullPath);

	IFileManager& FileManager = IFileManager::Get();

	if (!Ext.Contains(TEXT("*")))
	{
		if (Ext == "")
		{
			Ext = "*.*";
		}
		else
		{
			Ext = (Ext.Left(1) == ".") ? "*" + Ext : "*." + Ext;
		}
	}

	FString FinalPath = RootFolderFullPath + "/" + Ext;
	FileManager.FindFiles(Files, *FinalPath, true, false); 
	return true;
}

bool UMusicPlayerLibBPLibrary::GetFilesInRootAndAllSubFolders(TArray<FString>& Files, FString RootFolderFullPath, FString Ext)
{
	if (RootFolderFullPath.Len() < 1) return false;

	FPaths::NormalizeDirectoryName(RootFolderFullPath);

	IFileManager& FileManager = IFileManager::Get();

	if (!Ext.Contains(TEXT("*")))
	{
		if (Ext == "")
		{
			Ext = "*.*";
		}
		else
		{
			Ext = (Ext.Left(1) == ".") ? "*" + Ext : "*." + Ext;
		}
	}

	FileManager.FindFilesRecursive(Files, *RootFolderFullPath, *Ext, true, false);
	return true;
}

FString UMusicPlayerLibBPLibrary::GetGameRootDirectory()
{
	return FPaths::ConvertRelativePathToFull(FPaths::GameDir());
}

void UMusicPlayerLibBPLibrary::ExplodeString(TArray<FString>& OutputStrings, FString InputString, FString Separator, int32 limit, bool bTrimElements)
{
	OutputStrings.Empty();
	//~~~~~~~~~~~

	if (InputString.Len() > 0 && Separator.Len() > 0) {
		int32 StringIndex = 0;
		int32 SeparatorIndex = 0;

		FString Section = "";
		FString Extra = "";

		int32 PartialMatchStart = -1;

		while (StringIndex < InputString.Len()) {

			if (InputString[StringIndex] == Separator[SeparatorIndex]) {
				if (SeparatorIndex == 0) {
					//A new partial match has started.
					PartialMatchStart = StringIndex;
				}
				Extra.AppendChar(InputString[StringIndex]);
				if (SeparatorIndex == (Separator.Len() - 1)) {
					//We have matched the entire separator.
					SeparatorIndex = 0;
					PartialMatchStart = -1;
					if (bTrimElements == true) {
						OutputStrings.Add(FString(Section).Trim().TrimTrailing());
					}
					else {
						OutputStrings.Add(FString(Section));
					}

					//if we have reached the limit, stop.
					if (limit > 0 && OutputStrings.Num() >= limit)
					{
						return;
						//~~~~
					}

					Extra.Empty();
					Section.Empty();
				}
				else {
					++SeparatorIndex;
				}
			}
			else {
				//Not matched.
				//We should revert back to PartialMatchStart+1 (if there was a partial match) and clear away extra.
				if (PartialMatchStart >= 0) {
					StringIndex = PartialMatchStart;
					PartialMatchStart = -1;
					Extra.Empty();
					SeparatorIndex = 0;
				}
				Section.AppendChar(InputString[StringIndex]);
			}

			++StringIndex;
		}

		//If there is anything left in Section or Extra. They should be added as a new entry.
		if (bTrimElements == true) {
			OutputStrings.Add(FString(Section + Extra).Trim().TrimTrailing());
		}
		else {
			OutputStrings.Add(FString(Section + Extra));
		}

		Section.Empty();
		Extra.Empty();
	}
}


static EImageFormat::Type GetImageFormat(EMPLibImageFormats ImageFormat)
{

	switch (ImageFormat)
	{
	case EMPLibImageFormats::JPG: return EImageFormat::JPEG;
	case EMPLibImageFormats::PNG: return EImageFormat::PNG;
	case EMPLibImageFormats::BMP: return EImageFormat::BMP;
	case EMPLibImageFormats::ICO: return EImageFormat::ICO;
	case EMPLibImageFormats::EXR: return EImageFormat::EXR;
	case EMPLibImageFormats::ICNS: return EImageFormat::ICNS;
	}
	return EImageFormat::JPEG;
}

UTexture2D* UMusicPlayerLibBPLibrary::LoadTexture2D_FromFile(const FString& FullFilePath, EMPLibImageFormats ImageFormat, bool& IsValid, int32& Width, int32& Height)
{


	IsValid = false;
	UTexture2D* LoadedT2D = NULL;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	IImageWrapperPtr ImageWrapper = ImageWrapperModule.CreateImageWrapper(GetImageFormat(ImageFormat));

	//Load From File
	TArray<uint8> RawFileData;
	if (!FFileHelper::LoadFileToArray(RawFileData, *FullFilePath)) return NULL;
	//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

	//Create T2D!
	if (ImageWrapper.IsValid() && ImageWrapper->SetCompressed(RawFileData.GetData(), RawFileData.Num()))
	{
		const TArray<uint8>* UncompressedBGRA = NULL;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, UncompressedBGRA))
		{
			LoadedT2D = UTexture2D::CreateTransient(ImageWrapper->GetWidth(), ImageWrapper->GetHeight(), PF_B8G8R8A8);

			//Valid?
			if (!LoadedT2D) return NULL;
			//~~~~~~~~~~~~~~

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
	IsValid = true;
	return LoadedT2D;
}


class UAudioComponent* UMusicPlayerLibBPLibrary::PlaySoundAttachedFromFile(const FString& FilePath, class USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, EAttachLocation::Type LocationType, bool bStopWhenAttachedToDestroyed, float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings)
{
	USoundWave* sw = GetSoundWaveFromFile(FilePath);

	if (!sw)
		return NULL;

	return UGameplayStatics::SpawnSoundAttached(sw, AttachToComponent, AttachPointName, Location, LocationType, bStopWhenAttachedToDestroyed, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings);
}


void UMusicPlayerLibBPLibrary::PlaySoundAtLocationFromFile(UObject* WorldContextObject, const FString& FilePath, FVector Location, float VolumeMultiplier, float PitchMultiplier, float StartTime, class USoundAttenuation* AttenuationSettings)
{
	USoundWave* sw = GetSoundWaveFromFile(FilePath);

	if (!sw)
		return;

	UGameplayStatics::PlaySoundAtLocation(WorldContextObject, sw, Location, VolumeMultiplier, PitchMultiplier, StartTime, AttenuationSettings);
}


class USoundWave* UMusicPlayerLibBPLibrary::GetSoundWaveFromFile(const FString& FilePath)
{
#if PLATFORM_PS4
	UE_LOG(LogTemp, Error, TEXT("UMusicPlayerLibBPLibrary::GetSoundWaveFromFile ~ vorbis-method not supported on PS4. See UMusicPlayerLibBPLibrary::fillSoundWaveInfo"));
	return nullptr;
#endif
	
	USoundWave* sw = NewObject<USoundWave>(USoundWave::StaticClass());

	if (!sw)
		return NULL;

	//* If true the song was successfully loaded
	bool loaded = false;

	//* loaded song file (binary, encoded)
	TArray < uint8 > rawFile;

	loaded = FFileHelper::LoadFileToArray(rawFile, FilePath.GetCharArray().GetData());

	if (loaded)
	{
		FByteBulkData* bulkData = &sw->CompressedFormatData.GetFormat(TEXT("OGG"));

		bulkData->Lock(LOCK_READ_WRITE);
		FMemory::Memcpy(bulkData->Realloc(rawFile.Num()), rawFile.GetData(), rawFile.Num());
		bulkData->Unlock();

		loaded = fillSoundWaveInfo(sw, &rawFile) == 0 ? true : false;
	}

	if (!loaded)
		return NULL;

	return sw;
}


#if !PLATFORM_PS4
int32 UMusicPlayerLibBPLibrary::fillSoundWaveInfo(class USoundWave* sw, TArray<uint8>* rawFile)
{
	FSoundQualityInfo info;
	FVorbisAudioInfo vorbis_obj;
	if (!vorbis_obj.ReadCompressedInfo(rawFile->GetData(), rawFile->Num(), &info))
	{
		//Debug("Can't load header");
		return 1;
	}

	if (!sw) return 1;
	sw->SoundGroup = ESoundGroup::SOUNDGROUP_Default;
	sw->NumChannels = info.NumChannels;
	sw->Duration = info.Duration;
	sw->RawPCMDataSize = info.SampleDataSize;
	sw->SampleRate = info.SampleRate;
	

	return 0;
}
#endif //PLATFORM_PS4
