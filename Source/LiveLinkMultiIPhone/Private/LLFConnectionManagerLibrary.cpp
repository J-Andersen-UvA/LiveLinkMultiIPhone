#include "LLFConnectionManagerLibrary.h"

static ULLFConnectionManager* GConnectionManager = nullptr;

ULLFConnectionManager* ULLFConnectionManagerLibrary::GetConnectionManager()
{
    if (!GConnectionManager)
    {
        GConnectionManager = NewObject<ULLFConnectionManager>(GetTransientPackage());
        GConnectionManager->AddToRoot();
        GConnectionManager->Initialize();
    }

    return GConnectionManager;
}
