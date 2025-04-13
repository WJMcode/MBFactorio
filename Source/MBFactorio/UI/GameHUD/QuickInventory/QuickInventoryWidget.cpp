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
    UE_LOG(LogTemp, Warning, TEXT("InitSlots called. SlotCount = %d"), SlotCount);

    // 유효성 검사
    if (!SlotContainer || !SlotWidgetClass)
    {
        UE_LOG(LogTemp, Error, TEXT("SlotContainer or SlotWidgetClass is NULL!"));
        return;
    }

    // 기존 슬롯 제거
    SlotContainer->ClearChildren();
    Slots.Empty();

    // 슬롯 개수만큼 생성
    for (int32 i = 0; i < SlotCount; ++i)
    {
        UQuickInventorySlotWidget* NewSlot = CreateWidget<UQuickInventorySlotWidget>(this, SlotWidgetClass);
        if (NewSlot)
        {
            UHorizontalBoxSlot* NewBoxSlot = SlotContainer->AddChildToHorizontalBox(NewSlot);
            if (NewBoxSlot)
            {
                NewBoxSlot->SetSize(ESlateSizeRule::Fill);
                NewBoxSlot->SetPadding(FMargin(2.0f));
            }

            Slots.Add(NewSlot);
        }
    }

    UE_LOG(LogTemp, Warning, TEXT("Slots.Num() = %d"), Slots.Num());
}

int32 UQuickInventoryWidget::FindFirstEmptySlot() const
{
    for (int32 i = 0; i < Slots.Num(); ++i)
    {
        if (!Slots[i]->GetItem().IsValid())
        {
            return i;
        }
    }
    return -1;
}

bool UQuickInventoryWidget::SetFirstEmptySlotItem(FItemData& Item)
{
    int32 EmptyIndex = FindFirstEmptySlot();
    if (EmptyIndex != -1)
    {
        SetSlotItem(EmptyIndex, Item);
        return true;
    }
    return false;
}


void UQuickInventoryWidget::SetSlotItem(int32 Index, FItemData& Item)
{
    if (Slots.IsValidIndex(Index))
    {
        Slots[Index]->SetItem(Item);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("SetSlotItem: Invalid index %d"), Index);
    }
}

void UQuickInventoryWidget::ClearSlot(int32 Index)
{
    if (Slots.IsValidIndex(Index))
    {
        Slots[Index]->ClearItem();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("ClearSlot: Invalid index %d"), Index);
    }
}

FItemData UQuickInventoryWidget::GetSlotItem(int32 Index) const
{
    if (Slots.IsValidIndex(Index))
    {
        return Slots[Index]->GetItem();
    }

    return FItemData{};
}
