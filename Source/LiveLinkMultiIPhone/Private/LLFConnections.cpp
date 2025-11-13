#include "LLFConnections.h"

void ULLFConnections::SetActiveIPhone(FLLFDevice DeviceInfo)
{
    SetActiveIPhone(DeviceInfo.DeviceID);
}

void ULLFConnections::SetActiveIPhone(FString DeviceName)
{
    FLLFDevice Device = FindDeviceByName(DeviceName);

    if (Device.DeviceID != NAME_None)
    {
        SetActiveIPhone(Device.DeviceID);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Device with name %s not found"), *DeviceName);
    }
}

void ULLFConnections::SetActiveIPhone(FName DeviceID)
{
    // Check if device exists in discovered list
    for (const FLLFDevice& Device : Devices)
    {
        if (Device.DeviceID == DeviceID)
        {
            ActiveDevice = Device;
            OnActiveIPhoneChanged.Broadcast(DeviceID);
            return;
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Device %s not found in discovered devices"), *DeviceID.ToString());
}

void ULLFConnections::AddDevice(FLLFDevice Device)
{
    // Check if device already exists
    for (const FLLFDevice& ExistingDevice : Devices)
    {
        if (ExistingDevice.DeviceID == Device.DeviceID)
        {
            UE_LOG(LogTemp, Warning, TEXT("Device %s already discovered"), *Device.DeviceID.ToString());
            return;
        }
    }

    Devices.Add(Device);
    UE_LOG(LogTemp, Log, TEXT("Added device: %s"), *Device.DeviceID.ToString());
}

void ULLFConnections::RemoveDevice(FName DeviceID)
{
    int32 RemovedCount = Devices.RemoveAll([DeviceID](const FLLFDevice& Device)
    {
        return Device.DeviceID == DeviceID;
    });

    if (RemovedCount > 0)
    {
        UE_LOG(LogTemp, Log, TEXT("Removed device: %s"), *DeviceID.ToString());
        
        // If we removed the active device, clear it
        if (ActiveDevice.DeviceID == DeviceID)
        {
            ActiveDevice = FLLFDevice();
            OnActiveIPhoneChanged.Broadcast(NAME_None);
        }
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("Device %s not found for removal"), *DeviceID.ToString());
    }
}

void ULLFConnections::RemoveAllDevices()
{
    int32 DeviceCount = Devices.Num();
    Devices.Empty();
    ActiveDevice = FLLFDevice();
    
    UE_LOG(LogTemp, Log, TEXT("Removed all devices (%d total)"), DeviceCount);
    OnActiveIPhoneChanged.Broadcast(NAME_None);
}

void ULLFConnections::RemoveNonActiveDevices()
{
    if (ActiveDevice.DeviceID == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("No active device set, cannot remove non-active devices"));
        return;
    }

    FName ActiveID = ActiveDevice.DeviceID;
    int32 RemovedCount = Devices.RemoveAll([ActiveID](const FLLFDevice& Device)
    {
        return Device.DeviceID != ActiveID;
    });

    UE_LOG(LogTemp, Log, TEXT("Removed %d non-active devices"), RemovedCount);
}

void ULLFConnections::RemoveActiveDevice()
{
    if (ActiveDevice.DeviceID == NAME_None)
    {
        UE_LOG(LogTemp, Warning, TEXT("No active device to remove"));
        return;
    }

    FName ActiveID = ActiveDevice.DeviceID;
    RemoveDevice(ActiveID);
}

FLLFDevice ULLFConnections::FindDeviceByName(FString DeviceName)
{
    for (const FLLFDevice& Device : Devices)
    {
        if (Device.DeviceName == DeviceName)
        {
            return Device;
        }
    }

    return FLLFDevice(); // Return default if not found
}
