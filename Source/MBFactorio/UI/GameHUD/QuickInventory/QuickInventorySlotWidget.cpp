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

void UQuickInventorySlotWidget::SetItem(FItemData& InItem)
{
    SlotItem = InItem;

    if (ItemIcon)
    {
        ItemIcon->SetBrushFromTexture(SlotItem.Image);
        ItemIcon->SetVisibility(ESlateVisibility::Visible);
    }
    if (ItemCount)
    {
        ItemCount->SetText(FText::AsNumber(SlotItem.CreateCount));
        ItemCount->SetVisibility(SlotItem.CreateCount > 1 ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
    }

    SetVisibility(ESlateVisibility::Visible); // 슬롯 전체도 보이게
}

void UQuickInventorySlotWidget::ClearItem()
{
    SlotItem = FItemData();

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

const FItemData& UQuickInventorySlotWidget::GetItem() const
{
    return SlotItem;
}

bool UQuickInventorySlotWidget::HasItem() const
{
    return SlotItem.IsValid();

}

void UQuickInventorySlotWidget::HandleClick()
{
    if (!SlotItem.IsValid()) { return; }

    UItemCursorWidget* CursorWidgetInstance = GetItemCursorWidget();

    // 슬롯 → 커서
    if (CursorWidgetInstance && !CursorWidgetInstance->HasItem() && HasItem())
    {
        CursorWidgetInstance->SetItem(SlotItem);
        ClearItem();
        return;
    }

    // 커서 → 슬롯
    if (CursorWidgetInstance && CursorWidgetInstance->HasItem() && !HasItem())
    {
        SetItem(CursorWidgetInstance->GetItem());
        CursorWidgetInstance->ClearItem();
        return;
    }

    // 충돌 시 슬롯 비우기
    ClearItem();
}

UItemCursorWidget* UQuickInventorySlotWidget::GetItemCursorWidget() const
{
    ALYJController* LYJPC = Cast<ALYJController>(UGameplayStatics::GetPlayerController(this, 0));
    if (LYJPC)
    {
        return LYJPC->GetItemCursorWidget(); // 커서 위젯을 가져오는 함수가 있어야 함
    }

    return nullptr;
}
