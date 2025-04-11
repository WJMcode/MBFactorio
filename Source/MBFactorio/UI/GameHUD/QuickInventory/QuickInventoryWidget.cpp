#include "QuickInventoryWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

void UQuickInventoryWidget::NativeConstruct()
{
    Super::NativeConstruct();
    InitSlots(); // 슬롯 초기화
}

void UQuickInventoryWidget::InitSlots()
{
    if (!SlotContainer || !SlotWidgetClass) return;

    Slots.Empty(); // 이전 슬롯 제거

    for (int32 i = 0; i < SlotCount; ++i)
    {
        // 슬롯 위젯 생성
        UQuickInventorySlotWidget* NewSlot = CreateWidget<UQuickInventorySlotWidget>(this, SlotWidgetClass);
        if (NewSlot)
        {
            // 슬롯 컨테이너에 추가
            SlotContainer->AddChildToHorizontalBox(NewSlot);
            // 내부 배열에 저장
            Slots.Add(NewSlot);
        }
    }
}

void UQuickInventoryWidget::SetSlotItem(int32 Index, const FQuickItemData& Item)
{
    if (Slots.IsValidIndex(Index))
    {
        Slots[Index]->SetItem(Item);
    }
}

void UQuickInventoryWidget::ClearSlot(int32 Index)
{
    if (Slots.IsValidIndex(Index))
    {
        Slots[Index]->ClearItem();
    }
}

FQuickItemData UQuickInventoryWidget::GetSlotItem(int32 Index) const
{
    if (Slots.IsValidIndex(Index))
    {
        return Slots[Index]->GetItem();
    }

    return FQuickItemData();
}
