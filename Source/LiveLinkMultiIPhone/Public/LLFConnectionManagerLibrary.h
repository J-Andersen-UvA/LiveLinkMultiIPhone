#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "LLFConnectionManager.h"
#include "LLFConnectionManagerLibrary.generated.h"

UCLASS()
class LIVELINKMULTIIPHONE_API ULLFConnectionManagerLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, Category = "MultiIPhoneLiveLink")
    static ULLFConnectionManager* GetConnectionManager();
};
