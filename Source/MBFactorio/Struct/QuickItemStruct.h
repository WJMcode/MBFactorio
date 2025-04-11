#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuickItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FQuickItemData
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* Icon;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ItemActor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 Count;

    FQuickItemData()
        : Icon(nullptr), ItemActor(nullptr), Count(0)
    {
    }

    // QuickItemStruct.h
    FORCEINLINE bool IsValid() const
    {
        return Icon != nullptr && ItemActor != nullptr && Count > 0;
    }

};
