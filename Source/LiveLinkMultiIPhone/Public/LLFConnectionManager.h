#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LLFDeviceRegistry.h"
#include "LLFSourceDiscovery.h"
#include "LLFConnectionManager.generated.h"

UCLASS(BlueprintType)
class LIVELINKMULTIIPHONE_API ULLFConnectionManager : public UObject
{
    GENERATED_BODY()

public:
    // Registry for managed devices
    UPROPERTY()
    ULLFDeviceRegistry* DeviceRegistry;
    
    // Discovery for finding new sources
    UPROPERTY()
    ULLFSourceDiscovery* SourceDiscovery;
    
    void Initialize();
    
    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void DiscoverAndRegisterIPhones();
    
    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    ULLFDeviceRegistry* GetDeviceRegistry()
    { 
        if (!DeviceRegistry) Initialize();
        return DeviceRegistry; 
    }

    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    ULLFSourceDiscovery* GetSourceDiscovery() 
    { 
        if (!SourceDiscovery) Initialize();
        return SourceDiscovery; 
    }

    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void ActivateLiveLinkSubjectForDevice(FLLFDevice Device);

    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void DeactivateLiveLinkSubjectForDevice(FLLFDevice Device);
};
