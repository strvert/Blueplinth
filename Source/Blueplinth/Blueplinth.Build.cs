// Copyright Epic Games, Inc. All Rights Reserved.

using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using Tools.DotNETCommon;
using UnrealBuildTool;

public class Blueplinth : ModuleRules
{
	public Blueplinth(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[]
			{
				// ... add public include paths required here ...
			}
		);


		PrivateIncludePaths.AddRange(
			new string[]
			{
				// ... add other private include paths required here ...
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				// ... add other public dependencies that you statically link with here ...
			}
		);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
		);


		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);

		if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			bEnableExceptions = true;
			bUseUnity = false;
			CppStandard = CppStandardVersion.Cpp17;
			PublicSystemLibraries.AddRange(new[] {"shlwapi.lib", "runtimeobject.lib"});
			PrivateIncludePaths.Add(Path.Combine(Target.WindowsPlatform.WindowsSdkDir,
				"Include",
				Target.WindowsPlatform.WindowsSdkVersion,
				"cppwinrt"));
		}
	}
}