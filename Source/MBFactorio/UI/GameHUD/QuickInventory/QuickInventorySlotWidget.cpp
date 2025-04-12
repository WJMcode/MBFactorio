#include "QuickInventorySlotWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "ItemCursorWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Tools/LYJController.h"

void UQuickInventorySlotWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (SlotButton)
    {
        SlotButton->OnClicked.AddDynamic(this, &UQuickInventorySlotWidget::HandleClick);
    }
}

void UQuickInventorySlotWidget::SetItem(const FQuickItemData& InItem)
{
    SlotItem = InItem;

    if (SlotItem.IsValid())
    {
        if (ItemIcon)
        {
            ItemIcon->SetBrushFromTexture(SlotItem.ItemIcon);
            ItemIcon->SetVisibility(ESlateVisibility::Visible);
        }
        if (ItemCount)
        {
            ItemCount->SetText(FText::AsNumber(SlotItem.ItemCount));
            ItemCount->SetVisibility(SlotItem.ItemCount > 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
        }
    }
    else
    {
        ClearItem(); // 유효하지 않은 경우 바로 클리어
    }

    SetVisibility(ESlateVisibility::Visible); // 슬롯 전체도 보이게
}

void UQuickInventorySlotWidget::ClearItem()
{
    SlotItem = FQuickItemData();

    if (ItemIcon)
    {
        ItemIcon->SetBrushFromTexture(nullptr);
        ItemIcon->SetVisibility(ESlateVisibility::Collapsed);
    }
    if (ItemCount)
    {
        ItemCount->SetText(FText::GetEmpty());
        ItemCount->SetVisibility(ESlateVisibility::Collapsed);
    }
}

const FQuickItemData& UQuickInventorySlotWidget::GetItem() const
{
    return SlotItem;
}

bool UQuickInventorySlotWidget::HasItem() const
{
    return SlotItem.IsValid();

}

void UQuickInventorySlotWidget::HandleClick()
{
    if (!SlotItem.IsValid())
    {
        return;
    }

    //// 플레이어 컨트롤러 가져오기
    //if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
    //{
    //    if (ALYJController* LYJPC = Cast<ALYJController>(PC))
    //    {
    //        if (LYJPC->ItemCursorWidget)
    //        {
    //            // 커서 위젯에 아이템 설정 및 표시
    //            LYJPC->ItemCursorWidget->SetItem(SlotItem);
    //            LYJPC->ItemCursorWidget->SetVisibility(ESlateVisibility::Visible);

    //            UE_LOG(LogTemp, Warning, TEXT("커서에 아이템 부착: %s"), *SlotItem.ItemID);
    //        }
    //    }
    //}

    // 슬롯 초기화 (아이템 제거)
    ClearItem();
}
