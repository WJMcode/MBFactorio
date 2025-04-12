#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Tiles/TileBase/TileStructs.h"
#include "QuickItemStruct.generated.h"

USTRUCT(BlueprintType)
struct FQuickItemData
{
    GENERATED_BODY()

    // 식별용 문자열 ID
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString ItemID;

    // 아이템 아이콘
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UTexture2D* ItemIcon = nullptr;

    // 아이템 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    int32 ItemCount = 0;

    // 자원 타입 (예: 돌, 철 등)
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    EResourceType ResourceType = EResourceType::Unknown;

    // 아이템 설명 텍스트
    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FString Description;

    // 자동 초기화 함수
    void InitFromID(FString ID, UDataTable* DataTable);

    // 유효성 검사
    FORCEINLINE bool IsValid() const
    {
        return !ItemID.IsEmpty() && ItemIcon != nullptr && ItemCount > 0;
    }
};
