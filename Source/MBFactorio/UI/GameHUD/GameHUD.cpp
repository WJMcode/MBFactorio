#include "GameHUD.h"
#include "QuickInventory/QuickInventoryWidget.h"
#include "Quest/QuestWidget.h"
#include "ResourceInfo/ResourceInfoWidget.h"
#include "Struct/QuickItemStruct.h"
#include "Engine/DataTable.h"

void UGameHUD::NativeConstruct()
{
    Super::NativeConstruct();

    InitializeQuickInventory();
    UpdateQuestStatus(0, 0);
    ClearResourceInfo();

    {
        // 빈 정보로 초기화 (디자인은 보이되 내용 없음)
        FQuickItemData EmptyData;
        if (UI_ResourceInfo)
        {
            UI_ResourceInfo->SetResourceInfo(EmptyData);
        }

        // 데이터 테이블 로드 (런타임 안전 방식)
        ItemInfoDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/LYJ_Test/DT_ItemInfo.DT_ItemInfo"));
        if (ItemInfoDataTable)
        {
            UE_LOG(LogTemp, Warning, TEXT("ItemInfo 데이터 테이블 로드 성공"));
        }
        else
        {
            UE_LOG(LogTemp, Error, TEXT("ItemInfo 데이터 테이블 로드 실패"));
        }
    }   
}

void UGameHUD::ShowResourceInfoByID(const FString& ItemID)
{
    if (!ItemInfoDataTable || !UI_ResourceInfo) { return; }

    FQuickItemData Item;
    Item.InitFromID(ItemID, ItemInfoDataTable); // 여기서 자동으로 Description 등 채워짐
    UI_ResourceInfo->SetResourceInfo(Item);
}

void UGameHUD::InitializeQuickInventory()
{
    if (UI_QuickInventory)
    {
        UI_QuickInventory->InitSlots();
    }
}

void UGameHUD::UpdateQuestStatus(int32 DrillCount, int32 FurnaceCount)
{
    if (UI_Quest)
    {
        UI_Quest->UpdateQuestText(DrillCount, FurnaceCount);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("UI_Quest가 null입니다"));
    }
}

void UGameHUD::ShowResourceInfo(const FQuickItemData& ResourceInfo)
{
    if (UI_ResourceInfo)
    {
        UI_ResourceInfo->SetResourceInfo(ResourceInfo);
    }
}

void UGameHUD::ClearResourceInfo()
{
    if (UI_ResourceInfo)
    {
        UI_ResourceInfo->HideInfo();
    }
}
