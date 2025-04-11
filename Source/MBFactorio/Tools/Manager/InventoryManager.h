#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "struct/MBFStruct.h"
#include "InventoryManager.generated.h"

class UMBFInventoryComponent;

USTRUCT(BlueprintType)
struct FSelectedSlotData
{
    GENERATED_BODY()

    UPROPERTY()
    UMBFInventoryComponent* SourceInventory = nullptr;

    UPROPERTY()
    int32 SlotIndex = -1;

    UPROPERTY()
    FInventoryItem ItemData;

    bool IsValid() const
    {
        return SourceInventory != nullptr && SlotIndex >= 0 && !ItemData.IsEmpty();
    }

    void Clear()
    {
        SourceInventory = nullptr;
        SlotIndex = -1;
        ItemData = FInventoryItem();
    }
};

UCLASS(Blueprintable)
class MBFACTORIO_API UInventoryManager : public UObject
{
    GENERATED_BODY()

public:
    void Initialize();

    void SelectSlot(UMBFInventoryComponent* Inventory, int32 Index, const FInventoryItem& Item);
    void ClearSelection();

    const FSelectedSlotData& GetSelectedSlot() const { return SelectedSlot; }

    // 슬롯 클릭했을 때 처리 로직
    void OnSlotClicked(UMBFInventoryComponent* TargetInventory, int32 TargetIndex, const FInventoryItem& TargetItem);

private:
    FSelectedSlotData SelectedSlot;
};