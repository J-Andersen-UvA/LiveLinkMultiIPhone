using UnrealBuildTool;

public class LiveLinkMultiIPhoneEditor : ModuleRules
{
    public LiveLinkMultiIPhoneEditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine",
                "UnrealEd",
                "LiveLinkInterface",
                "LiveLinkMultiIPhone" // depend on runtime
            }
        );
    }
}
