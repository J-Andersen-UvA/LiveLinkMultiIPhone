#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LLFConnections.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveIPhoneChanged, FName, DeviceID);

USTRUCT(BlueprintType)
struct FLLFDevice
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FName DeviceID;

    UPROPERTY(BlueprintReadOnly)
    bool bIsConnected = false;
    
    UPROPERTY(BlueprintReadOnly)
    FString DeviceName;

    UPROPERTY(BlueprintReadOnly)
    FString DeviceIPAddress;
};

UCLASS(BlueprintType, Blueprintable)
class LIVELINKMULTIIPHONE_API ULLFConnections : public UObject
{
    GENERATED_BODY()

public:
    // Store devices
    UPROPERTY(BlueprintReadOnly, Category = "MultiIphoneLiveLink")
    TArray<FLLFDevice> Devices;

    // Track the currently active device
    UPROPERTY(BlueprintReadOnly, Category = "MultiIphoneLiveLink")
    FLLFDevice ActiveDevice;

    // Blueprint-assignable event
    UPROPERTY(BlueprintAssignable, Category = "MultiIphoneLiveLink")
    FOnActiveIPhoneChanged OnActiveIPhoneChanged;

    // Functions to set active iPhone
    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void SetActiveIPhone(FLLFDevice Device);
    // Overload for setting active iPhone by DeviceID and DeviceName
    void SetActiveIPhone(FName DeviceID);
    void SetActiveIPhone(FString DeviceName);

    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void AddDevice(FLLFDevice Device);

    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void RemoveDevice(FName DeviceID);

    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void RemoveAllDevices();

    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void RemoveNonActiveDevices();

    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void RemoveActiveDevice();

    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    FLLFDevice FindDeviceByName(FString DeviceName);
};