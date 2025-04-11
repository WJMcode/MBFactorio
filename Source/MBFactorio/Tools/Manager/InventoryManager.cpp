#include "InventoryManager.h"
#include "Component/MBFInventoryComponent.h"

void UInventoryManager::Initialize()
{
    SelectedSlot.Clear();
}

void UInventoryManager::SelectSlot(UMBFInventoryComponent* Inventory, int32 Index, const FInventoryItem& Item)
{
    SelectedSlot.SourceInventory = Inventory;
    SelectedSlot.SlotIndex = Index;
    SelectedSlot.ItemData = Item;
}

void UInventoryManager::ClearSelection()
{
    SelectedSlot.Clear();
}

void UInventoryManager::OnSlotClicked(UMBFInventoryComponent* TargetInventory, int32 TargetIndex, const FInventoryItem& TargetItem)
{
    if (!SelectedSlot.IsValid())
    {
        // 아무것도 선택 안된 상태 → 지금 슬롯 선택
        SelectSlot(TargetInventory, TargetIndex, TargetItem);
        return;
    }

    if (SelectedSlot.SourceInventory == TargetInventory && SelectedSlot.SlotIndex == TargetIndex)
    {
        // 같은 슬롯을 다시 클릭 → 선택 해제
        ClearSelection();
        return;
    }

    // 교환 or 이동 처리
    TargetInventory->TrySwapOrMoveItems(
        SelectedSlot.SlotIndex,
        TargetIndex,
        SelectedSlot.SourceInventory,
        &SelectedSlot.ItemData
    );

    // 선택 해제
    ClearSelection();
}