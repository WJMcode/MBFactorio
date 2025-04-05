#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "MBFStruct.generated.h"

// 합성에 필요한 아이템 정보를 담을 구조체



USTRUCT(BlueprintType)
struct FInventoryItem
{
    GENERATED_USTRUCT_BODY()
public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
    int32 ItemID;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
    int32 MaxCount;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item")
    int32 MCount;

    FInventoryItem()
        : ItemID(0), MaxCount(0), MCount(0)
    { }
};

USTRUCT(BlueprintType)
struct FRequiredItem
{
    GENERATED_USTRUCT_BODY()

public:
    // 아이템 ID (고유 번호)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Required Item")
    int32 ItemID;

    // 해당 아이템의 요구 개수
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Required Item")
    int32 RequiredCount;

    // 기본 생성자
    FRequiredItem()
        : ItemID(0), RequiredCount(0)
    {
    }
};

// 데이터 테이블에 사용할 아이템 데이터 구조체
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()


public:
    // 아이템 ID (고유 번호)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;

    // 아이템 조합시 생성되는 수량
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 CreateCount;

    // 아이템 최대 수량 (들고 있을 수 있는 수량 제한)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxCount;

    // 아이템 이미지 (Texture2D 포인터)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Image;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 BuildTime;

    // 아이템 합성 시 필요한 다른 아이템들 (아이템 ID와 요구 개수를 가진 배열)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TArray<FRequiredItem> RequiredItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float Energy;

    // 캐릭터 제작 가능 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bMake;
    // 연료 사용 가능 여부
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bFire;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool bCook;

    // 기본 생성자
    FItemData()
        : ItemID(0), MaxCount(0), Image(nullptr)
    {
    }
};