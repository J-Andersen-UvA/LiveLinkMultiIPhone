#include "LLFSourceDiscovery.h"
#include "LLFDeviceRegistry.h"  // For FLLFDevice struct
#include "Features/IModularFeatures.h"
#include "LiveLinkTypes.h"
#include "Modules/ModuleManager.h"

ILiveLinkClient* ULLFSourceDiscovery::GetLiveLinkClient()
{
    IModuleInterface* Module = FModuleManager::Get().GetModule("LiveLink");
    if (Module)
    {
        return &IModularFeatures::Get().GetModularFeature<ILiveLinkClient>(ILiveLinkClient::ModularFeatureName);
    }

    return nullptr;
}

TArray<FLLFDevice> ULLFSourceDiscovery::GetAvailableIPhones()
{
    TArray<FLLFDevice> iPhones;
    
    ILiveLinkClient* Client = GetLiveLinkClient();
    if (!Client)
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get LiveLink client"));
        return iPhones;
    }
    
    // Get all Live Link sources
    TArray<FGuid> SourceGuids = Client->GetSources();
    
    // Filter for iPhone sources
    for (const FGuid& SourceGuid : SourceGuids)
    {
        if (IsIPhoneSource(SourceGuid, Client))
        {
            FLLFDevice Device = CreateDeviceFromSource(SourceGuid, Client);
            iPhones.Add(Device);
        }
    }
    
    UE_LOG(LogTemp, Log, TEXT("Found %d iPhone Live Link sources"), iPhones.Num());
    return iPhones;
}

bool ULLFSourceDiscovery::IsIPhoneSource(const FGuid& SourceGuid, ILiveLinkClient* Client)
{
    if (!Client)
        return false;
    
    // Get source type/name
    FText SourceType = Client->GetSourceType(SourceGuid);
    FString SourceTypeName = SourceType.ToString();
    
    // Check if it's an iOS/iPhone source
    // Patterns to look for: "iPhone", "iOS", "Apple ARKit Face", etc.
    if (SourceTypeName.Contains(TEXT("iPhone")) ||
        SourceTypeName.Contains(TEXT("iOS")) ||
        SourceTypeName.Contains(TEXT("Apple ARKit")))
    {
        return true;
    }
    
    // Alternative: Check subject names for iPhone patterns
    TArray<FLiveLinkSubjectKey> Subjects = Client->GetSubjects(true, false);
    for (const FLiveLinkSubjectKey& Subject : Subjects)
    {
        if (Subject.Source == SourceGuid)
        {
            FString SubjectName = Subject.SubjectName.ToString();
            if (SubjectName.Contains(TEXT("iPhone")) || 
                SubjectName.Contains(TEXT("iOS")))
            {
                return true;
            }
        }
    }
    
    return false;
}

FLLFDevice ULLFSourceDiscovery::CreateDeviceFromSource(const FGuid& SourceGuid, ILiveLinkClient* Client)
{
    FLLFDevice Device;
    
    if (!Client)
        return Device;
    
    // Get source machine name (may contain device info)
    FText MachineName = Client->GetSourceMachineName(SourceGuid);
    FString MachineNameStr = MachineName.ToString();
    
    // Get source type for additional info
    FText SourceType = Client->GetSourceType(SourceGuid);
    
    // Try to parse device info
    FString DeviceName, DeviceID;
    if (ParseIPhoneSourceInfo(MachineNameStr, DeviceName, DeviceID))
    {
        Device.DeviceName = DeviceName;
        Device.DeviceID = FName(*DeviceID);
    }
    else
    {
        // Fallback: use source GUID as ID
        Device.DeviceID = FName(*SourceGuid.ToString());
        Device.DeviceName = MachineNameStr.IsEmpty() ? SourceType.ToString() : MachineNameStr;
    }
    
    // Check if source is currently connected
    Device.bIsConnected = Client->IsSourceStillValid(SourceGuid);
    
    // Try to get IP address from source settings (if available)
    ULiveLinkSourceSettings* Settings = Client->GetSourceSettings(SourceGuid);
    if (Settings)
    {
        // Extract connection info if exposed
        // This depends on the specific Live Link source implementation
        Device.DeviceIPAddress = TEXT("Unknown"); // Placeholder
    }
    
    return Device;
}

bool ULLFSourceDiscovery::ParseIPhoneSourceInfo(const FString& SourceName, FString& OutDeviceName, FString& OutDeviceID)
{
    // Expected patterns:
    // "John's iPhone" → Name: "John's iPhone", ID: "JohnsiPhone"
    // "iPhone_12345" → Name: "iPhone", ID: "iPhone_12345"
    
    if (SourceName.IsEmpty())
        return false;
    
    OutDeviceName = SourceName;
    OutDeviceID = SourceName.Replace(TEXT(" "), TEXT("")).Replace(TEXT("'"), TEXT(""));
    
    return true;
}