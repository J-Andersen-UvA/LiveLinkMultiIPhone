#include "LLFSourceDiscovery.h"
#include "LLFDeviceRegistry.h"  // For FLLFDevice struct
#include "Features/IModularFeatures.h"
#include "LiveLinkTypes.h"
#include "Modules/ModuleManager.h"

ILiveLinkClient* ULLFSourceDiscovery::GetLiveLinkClient()
{
    if (!FModuleManager::Get().IsModuleLoaded("LiveLink"))
    {
        FModuleManager::Get().LoadModule("LiveLink");
    }

    if (IModularFeatures::Get().IsModularFeatureAvailable(ILiveLinkClient::ModularFeatureName))
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
            TArray<FName> SubjectNames = GetAllSubjectNamesFromGUID(SourceGuid, Client);
            
            for (const FName& SubjectName : SubjectNames)
            {
                FLLFDevice Device = CreateDeviceFromSource(SourceGuid, SubjectName, Client);
                iPhones.Add(Device);
            }
        }
    }
    
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

FLLFDevice ULLFSourceDiscovery::CreateDeviceFromSource(const FGuid& SourceGuid, FName SubjectName, ILiveLinkClient* Client)
{
    FLLFDevice Device;
    
    if (!Client)
        return Device;
    
    // Store the subject name (passed as parameter)
    Device.SubjectName = SubjectName.ToString();
    
    // Create unique DeviceID from source GUID + subject name
    Device.DeviceID = FName(*FString::Printf(TEXT("%s_%s"), 
        *SourceGuid.ToString(EGuidFormats::Short), 
        *SubjectName.ToString()));
    
    Device.bIsConnected = Client->IsSourceStillValid(SourceGuid);
    Device.LiveLinkSourceGuid = SourceGuid;

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

TArray<FName> ULLFSourceDiscovery::GetAllSubjectNamesFromGUID(const FGuid& SourceGuid, ILiveLinkClient* Client)
{
    TArray<FName> SubjectNames;
    
    if (!Client)
        return SubjectNames;
    
    // Get all subjects (enabled and disabled)
    TArray<FLiveLinkSubjectKey> AllSubjects = Client->GetSubjects(true, true);
    
    // Filter subjects that belong to this source
    for (const FLiveLinkSubjectKey& SubjectKey : AllSubjects)
    {
        if (SubjectKey.Source == SourceGuid)
        {
            SubjectNames.Add(SubjectKey.SubjectName.Name);
        }
    }
    
    return SubjectNames;
}
