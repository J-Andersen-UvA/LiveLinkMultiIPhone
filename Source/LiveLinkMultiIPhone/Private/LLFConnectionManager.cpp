#include "LLFConnectionManager.h"
#include "Features/IModularFeatures.h"
#include "ILiveLinkClient.h"
#include "LiveLinkTypes.h"

void ULLFConnectionManager::Initialize()
{
    if (!DeviceRegistry)
    {
        DeviceRegistry = NewObject<ULLFDeviceRegistry>(GetTransientPackage());
    }
    
    if (!SourceDiscovery)
    {
        SourceDiscovery = NewObject<ULLFSourceDiscovery>(GetTransientPackage());
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

void ULLFConnectionManager::ActivateLiveLinkSubjectForDevice(FLLFDevice Device)
{
    ILiveLinkClient* Client = SourceDiscovery->GetLiveLinkClient();
    if (Client && Device.LiveLinkSourceGuid.IsValid())
    {
        // Create subject key from device info
        FLiveLinkSubjectKey SubjectKey;
        SubjectKey.Source = Device.LiveLinkSourceGuid;
        SubjectKey.SubjectName = FLiveLinkSubjectName(FName(*Device.SubjectName));
        
        // Enable this specific subject
        Client->SetSubjectEnabled(SubjectKey, true);
        UE_LOG(LogTemp, Log, TEXT("Activated subject '%s' for device: %s"), 
            *Device.SubjectName, *Device.DeviceID.ToString());
    }
}

void ULLFConnectionManager::DeactivateLiveLinkSubjectForDevice(FLLFDevice Device)
{
    ILiveLinkClient* Client = SourceDiscovery->GetLiveLinkClient();
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