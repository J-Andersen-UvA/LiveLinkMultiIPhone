#include "LLFEditorSubsystem.h"
#include "LLFConnectionManagerLibrary.h"

void ULLFEditorSubsystem::Initialize()
{
}

void ULLFEditorSubsystem::Deinitialize()
{
}

void ULLFEditorSubsystem::Tick(float DeltaTime)
{
    // Early exit if shutting down
    if (!IsValid(this) || IsEngineExitRequested())
    {
        return;
    }

    TimeAccumulator += DeltaTime;

    if (TimeAccumulator >= 5.0f)
    {
        TimeAccumulator = 0.f;

        if (ULLFConnectionManager* Manager = ULLFConnectionManagerLibrary::GetConnectionManager())
        {
            Manager->DiscoverAndRegisterIPhones();
        }
    }
}

TStatId ULLFEditorSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(ULLFEditorSubsystem, STATGROUP_Tickables);
}
