#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ILiveLinkClient.h"
#include "LiveLinkSourceSettings.h"
#include "LLFSourceDiscovery.generated.h"

struct FLLFDevice;

UCLASS()
class LIVELINKMULTIIPHONE_API ULLFSourceDiscovery : public UObject
{
    GENERATED_BODY()

public:
    // Main discovery function
    TArray<FLLFDevice> GetAvailableIPhones();

private:
    ILiveLinkClient* GetLiveLinkClient();
    
    bool IsIPhoneSource(const FGuid& SourceGuid, ILiveLinkClient* Client);
    
    // Convert Live Link source to FLLFDevice
    FLLFDevice CreateDeviceFromSource(const FGuid& SourceGuid, ILiveLinkClient* Client);
    
    bool ParseIPhoneSourceInfo(const FString& SourceName, FString& OutDeviceName, FString& OutDeviceID);
};
