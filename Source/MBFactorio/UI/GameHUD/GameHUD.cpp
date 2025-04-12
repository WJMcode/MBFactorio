#include "GameHUD.h"
#include "QuickInventory/QuickInventoryWidget.h"
#include "Quest/QuestWidget.h"
#include "ResourceInfo/ResourceInfoWidget.h"

void UGameHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // 필요한 경우 위젯 초기화
    InitializeQuickInventory();
}

void UGameHUD::InitializeQuickInventory()
{
    if (UI_QuickInventory)
    {
        UI_QuickInventory->InitSlots(); // 예시: 슬롯 초기화 함수
    }
}

void UGameHUD::ClearResourceInfo()
{
    if (UI_ResourceInfo)
    {
        UI_ResourceInfo->HideInfo(); // 값만 지우고 창은 유지
    }
}
