// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ELLE : ModuleRules
{
	public ELLE(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "Http", "Json", "JsonUtilities", "ZipUtility" });
       // PublicAdditionalLibraries.Add(@"D:/UnrealEngine/Projects/ELLE3/Binaries/Win32/libogg_static.lib");
       // PublicAdditionalLibraries.Add(@"D:/UnrealEngine/Projects/ELLE3/Binaries/Win32/libvorbis_static.lib");
       // PublicAdditionalLibraries.Add(@"D:/UnrealEngine/Projects/ELLE3/Binaries/Win32/libvorbisfile_static.lib");
       // MinFilesUsingPrecompiledHeaderOverride = 1;
       // bFasterWithoutUnity = true;
    }
}
