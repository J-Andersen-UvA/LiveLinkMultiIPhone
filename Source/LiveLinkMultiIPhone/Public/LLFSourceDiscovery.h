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
    ILiveLinkClient* GetLiveLinkClient();

private:
    bool IsIPhoneSource(const FGuid& SourceGuid, ILiveLinkClient* Client);
    
    // Convert Live Link source to FLLFDevice
    FLLFDevice CreateDeviceFromSource(const FGuid& SourceGuid, FName SubjectName, ILiveLinkClient* Client);
    TArray<FName> GetAllSubjectNamesFromGUID(const FGuid& SourceGuid, ILiveLinkClient* Client);
};
