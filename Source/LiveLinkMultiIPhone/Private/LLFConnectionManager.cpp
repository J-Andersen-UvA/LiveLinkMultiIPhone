#include "LLFConnectionManager.h"

void ULLFConnectionManager::Initialize()
{
    if (!DeviceRegistry)
    {
        DeviceRegistry = NewObject<ULLFDeviceRegistry>(this);
    }
    
    if (!SourceDiscovery)
    {
        SourceDiscovery = NewObject<ULLFSourceDiscovery>(this);
    }
}

void ULLFConnectionManager::DiscoverAndRegisterIPhones()
{
    if (!SourceDiscovery || !DeviceRegistry)
    {
        Initialize();
    }
    
    TArray<FLLFDevice> DiscoveredDevices = SourceDiscovery->GetAvailableIPhones();
    
    for (const FLLFDevice& Device : DiscoveredDevices)
    {
        DeviceRegistry->AddDevice(Device);
    }
    
    UE_LOG(LogTemp, Log, TEXT("Discovered and registered %d iPhones"), DiscoveredDevices.Num());
}