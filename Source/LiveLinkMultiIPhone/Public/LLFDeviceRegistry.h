#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "LLFDeviceRegistry.generated.h"

USTRUCT(BlueprintType)
struct FLLFDevice
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FName DeviceID;

    UPROPERTY(BlueprintReadOnly)
    bool bIsConnected = false;
    
    UPROPERTY(BlueprintReadOnly)
    FString SubjectName;

    UPROPERTY(BlueprintReadOnly)
    FString DeviceIPAddress;

    UPROPERTY(BlueprintReadOnly)
    FGuid LiveLinkSourceGuid;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnActiveIPhoneChanged, FName, DeviceID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeviceActivated, FLLFDevice, ActivatedDevice);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDeviceDeactivated, FLLFDevice, DeactivatedDevice);

UCLASS(BlueprintType, Blueprintable)
class LIVELINKMULTIIPHONE_API ULLFDeviceRegistry : public UObject
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

    UPROPERTY(BlueprintAssignable, Category = "MultiIphoneLiveLink")
    FOnDeviceActivated OnDeviceActivated;

    UPROPERTY(BlueprintAssignable, Category = "MultiIphoneLiveLink")
    FOnDeviceDeactivated OnDeviceDeactivated;

    // Functions to set active iPhone
    UFUNCTION(BlueprintCallable, Category = "MultiIphoneLiveLink")
    void SetActiveIPhone(FLLFDevice Device);
    // Overload for setting active iPhone by DeviceID and SubjectName
    void SetActiveIPhone(FName DeviceID);
    void SetActiveIPhone(FString SubjectName);

    // Functions to manage devices
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
    FLLFDevice FindDeviceBySubjectName(FString SubjectName);

    // Blueprint-friendly event callbacks (bind these in BP to get notified)
    UFUNCTION(BlueprintImplementableEvent, Category = "MultiIphoneLiveLink")
    void OnDeviceActivatedBP(const FLLFDevice& Device);

    UFUNCTION(BlueprintImplementableEvent, Category = "MultiIphoneLiveLink")
    void OnDeviceDeactivatedBP(const FLLFDevice& Device);

    UPROPERTY(BlueprintReadWrite, Category = "MultiIphoneLiveLink")
    bool bAutoManageLiveLinkSubjects = true;
};
