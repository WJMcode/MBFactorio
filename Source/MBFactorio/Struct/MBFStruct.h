#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "UObject/NoExportTypes.h"
#include "MBFStruct.generated.h"

// �ռ��� �ʿ��� ������ ������ ���� ����ü
USTRUCT(BlueprintType)
struct FRequiredItem
{
    GENERATED_USTRUCT_BODY()

public:
    // ������ ID (���� ��ȣ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Required Item")
    int32 ItemID;

    // �ش� �������� �䱸 ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Required Item")
    int32 RequiredCount;

    // �⺻ ������
    FRequiredItem()
        : ItemID(0), RequiredCount(0)
    {
    }
};

// ������ ���̺� ����� ������ ������ ����ü
USTRUCT(BlueprintType)
struct FItemData : public FTableRowBase
{
    GENERATED_USTRUCT_BODY()


public:
    // ������ ID (���� ��ȣ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 ItemID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    FString ItemName;

    // ������ ���ս� �����Ǵ� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 CreateCount;

    // ������ �ִ� ���� (��� ���� �� �ִ� ���� ����)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 MaxCount;

    // ������ �̹��� (Texture2D ������)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    UTexture2D* Image;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    int32 BuildTime;

    // ������ �ռ� �� �ʿ��� �ٸ� �����۵� (������ ID�� �䱸 ������ ���� �迭)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    TArray<FRequiredItem> RequiredItems;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    float Energy;

    // ĳ���� ���� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool CHMake;
    // ���� ��� ���� ����
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
    bool CUFire;

    // �⺻ ������
    FItemData()
        : ItemID(0), MaxCount(0), Image(nullptr)
    {
    }
};