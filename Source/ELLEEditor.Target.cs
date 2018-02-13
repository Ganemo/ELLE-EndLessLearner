// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ELLEEditorTarget : TargetRules
{
	public ELLEEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
        ExtraModuleNames.Add("ELLE");
	}

	//
	// TargetRules interface.
	//

	//public override void SetupBinaries(
	//	TargetInfo Target,
	//	ref List<UEBuildBinaryConfiguration> OutBuildBinaryConfigurations,
	//	ref List<string> OutExtraModuleNames
	//	)
	//{
	//	OutExtraModuleNames.Add("ELLE");
	//}
}
