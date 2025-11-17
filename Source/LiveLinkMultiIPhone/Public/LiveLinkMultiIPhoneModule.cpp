#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FLiveLinkMultiIPhoneModule : public IModuleInterface
{
public:
    virtual void StartupModule() override
    {
        UE_LOG(LogTemp, Log, TEXT("LiveLinkMultiIPhone: StartupModule"));
    }

    virtual void ShutdownModule() override
    {
        UE_LOG(LogTemp, Log, TEXT("LiveLinkMultiIPhone: ShutdownModule"));
    }
};

IMPLEMENT_MODULE(FLiveLinkMultiIPhoneModule, LiveLinkMultiIPhone)
