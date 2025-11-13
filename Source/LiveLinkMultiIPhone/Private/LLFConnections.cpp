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
