#pragma once
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "TickableEditorObject.h"
#include "Tickable.h"
#include "LLFEditorSubsystem.generated.h"

UCLASS()
class LIVELINKMULTIIPHONEEDITOR_API ULLFEditorSubsystem
    : public UObject
    , public FTickableEditorObject
{
    GENERATED_BODY()

public:
    virtual void Initialize();
    virtual void Deinitialize();

    // TickableEditorObject
    virtual void Tick(float DeltaTime) override;
    virtual TStatId GetStatId() const override;

    virtual bool IsTickable() const override { return true; }
    virtual bool IsEditorOnly() const override { return true; }

private:
    float TimeAccumulator = 0.f;
};
