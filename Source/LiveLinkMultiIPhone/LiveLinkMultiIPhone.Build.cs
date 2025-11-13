using UnrealBuildTool;

public class LiveLinkMultiIPhone : ModuleRules
{
    public LiveLinkMultiIPhone(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "Slate", "SlateCore", "LiveLink", "LiveLinkInterface" });
        PrivateDependencyModuleNames.AddRange(new string[] { "UnrealEd" });
    }
}
