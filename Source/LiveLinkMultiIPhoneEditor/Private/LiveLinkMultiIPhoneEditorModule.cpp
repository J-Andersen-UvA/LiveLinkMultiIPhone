#include "Modules/ModuleManager.h"
#include "LLFEditorSubsystem.h"

class FLiveLinkMultiIPhoneEditorModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        EditorSubsystem = NewObject<ULLFEditorSubsystem>();
        EditorSubsystem->AddToRoot();
        EditorSubsystem->Initialize();
    }

    virtual void ShutdownModule() override
    {
        if (EditorSubsystem)
        {
            // Try to cleanup, but don't crash if already destroyed
            if (!GExitPurge && EditorSubsystem->IsValidLowLevel() && !EditorSubsystem->IsUnreachable())
            {
                EditorSubsystem->Deinitialize();
                EditorSubsystem->RemoveFromRoot();
            }
            EditorSubsystem = nullptr;
        }
    }

private:
    ULLFEditorSubsystem* EditorSubsystem = nullptr;
};

IMPLEMENT_MODULE(FLiveLinkMultiIPhoneEditorModule, LiveLinkMultiIPhoneEditor)
