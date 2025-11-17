#include "LLFConnectionManager.h"
#include "Features/IModularFeatures.h"
#include "ILiveLinkClient.h"
#include "LiveLinkTypes.h"

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
    }

void ULLFConnectionManager::ActivateLiveLinkSubjectForDevice(const FLLFDevice& Device)
{
    ILiveLinkClient* Client = SourceDiscovery->GetLiveLinkClient(); // Need to make this public
    if (Client && Device.LiveLinkSourceGuid.IsValid())
    {
        // Get subjects for this source
        TArray<FLiveLinkSubjectKey> Subjects = Client->GetSubjects(true, false);
        for (const FLiveLinkSubjectKey& Subject : Subjects)
        {
            if (Subject.Source == Device.LiveLinkSourceGuid)
            {
                Client->SetSubjectEnabled(Subject, true);
                UE_LOG(LogTemp, Log, TEXT("Activated Live Link subject for device: %s"), *Device.DeviceID.ToString());
            }
        }
    }
}

void ULLFConnectionManager::DeactivateLiveLinkSubjectForDevice(const FLLFDevice& Device)
{
    ILiveLinkClient* Client = SourceDiscovery->GetLiveLinkClient(); // Need to make this public
    if (Client && Device.LiveLinkSourceGuid.IsValid())
    {
        TArray<FLiveLinkSubjectKey> Subjects = Client->GetSubjects(true, false);
        for (const FLiveLinkSubjectKey& Subject : Subjects)
        {
            if (Subject.Source == Device.LiveLinkSourceGuid)
            {
                Client->SetSubjectEnabled(Subject, false);
                UE_LOG(LogTemp, Log, TEXT("Deactivated Live Link subject for device: %s"), *Device.DeviceID.ToString());
            }
        }
    }
}