#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "QuickItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FQuickItemData
{
    GENERATED_BODY()

    // 식별용 문자열 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* ItemIcon = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TSubclassOf<AActor> ItemActor = nullptr;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemCount = 0;

    FQuickItemData()
        : ItemIcon(nullptr), ItemActor(nullptr), ItemCount(0)
    {
    }

    // QuickItemStruct.h
    FORCEINLINE bool IsValid() const
    {
        return !ItemID.IsEmpty() && ItemIcon != nullptr && ItemActor != nullptr && ItemCount > 0;
    }

};
