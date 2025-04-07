// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MBFInventoryComponent.h"
#include "Tools/MBFInstance.h"
#include "Struct/MBFStruct.h"
#include "Tools/MBFHUD.h"
#include "Math/UnrealMathUtility.h"

// Sets default values for this component's properties
UMBFInventoryComponent::UMBFInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InventoryItems.SetNum(80);
	for (int i = 0; i < 80; i++)
	{
		InventoryItems[i] = FInventoryItem(i + 1, i + 1, i + 1);
	}
	// ...
}


// Called when the game starts
void UMBFInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UMBFInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// Fill out your copyright notice in the Description page of Project Settings.






// 아이템 제작
bool UMBFInventoryComponent::CanCraftItem(int32 ItemID, int32 CraftCount, TMap<FName, int32>* OutMap, TArray<FName>* OutRequiredCraftings)
{
	
	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());
	const FItemData* Itemdata = Instance->GetItemData(FName(FString::FromInt(ItemID)));

	if (!Itemdata || !Itemdata->bCook) { return false; }

	TMap<FName, int32> LocalMap;
	TArray<FName> LocalRequiredCraftings;
	TMap<int32, TPair<int32, int32>> BasicMaterials;


	TMap<FName, int32>& MapRef = OutMap ? *OutMap : LocalMap;
	TArray<FName>& CraftingsRef = OutRequiredCraftings ? *OutRequiredCraftings : LocalRequiredCraftings;


	RequiredItemsCheck(MapRef, CraftingsRef, BasicMaterials, ItemID, TPair<int32, int32>(CraftCount, CraftCount));

	for (auto& BasicMaterial : BasicMaterials)
	{
		if (CheckMaterialAvailability(BasicMaterial.Key, BasicMaterial.Value.Value) != EMaterialCheckResult::Success)
			return false;
		// 경고 문구 추가시 요기에
	}

	return true;


}
void UMBFInventoryComponent::CraftItem(int32 ItemID, int32 CraftCount)
{
	TMap<FName, int32> Map;
	TArray<FName> RequiredItems;
	if (CanCraftItem(ItemID, CraftCount, &Map, &RequiredItems))
	{
		for (auto& RequiredItem : RequiredItems)
		{
			int32* ValuePtr = Map.Find(RequiredItem);
			if (ValuePtr)
			{
				TPair<FName, int32> Crafting(RequiredItem, *ValuePtr);
				Craftings.Add(Crafting);
			}
		}
	}

}
void UMBFInventoryComponent::Crafting(int32 ItemID)
{
	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());

	const FItemData* ItemData = Instance->GetItemData(FName(FString::FromInt(ItemID)));

	TArray<FRequiredItem> RequiredItems = ItemData->RequiredItems;

	for (auto RequiredItem : RequiredItems)
	{
		RemoveItem(RequiredItem.ItemID, RequiredItem.RequiredCount);
	}
	AddItem(ItemID, ItemData->CreateCount);
}
void UMBFInventoryComponent::AddItem(int32 ItemID, int32 Count)
{
	int32 minSlot = -1;
	for (int i = 0; i < 80; i++)
	{
		if (InventoryItems[i].ItemID == ItemID)
		{
			int32 CanAdd = InventoryItems[i].MaxCount - InventoryItems[i].MCount;
			if (CanAdd < Count)
			{
				InventoryItems[i].MCount = InventoryItems[i].MaxCount;
				Count -= CanAdd;
			}
			else
			{
				InventoryItems[i].MCount += Count;
				return;
			}
		}
		else if (InventoryItems[i].ItemID == 0)
		{
			minSlot = i;
			break;
		}
	}
	if (minSlot != -1)
	{
		UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());
		const FItemData* ItemData = Instance->GetItemData(FName(FString::FromInt(ItemID)));
		InventoryItems[minSlot] = FInventoryItem(ItemID, Count, ItemData->MaxCount);


	}
	else
	{
		//칸이 없을경우 바닥에 타일로 생성
	}


}
void UMBFInventoryComponent::RemoveItem(int32 ItemID, int32 Count)
{
	int32 need = Count;
	for (int i = 79; i >= 0; i--)
	{
		if (InventoryItems[i].ItemID == ItemID)
		{
			if (InventoryItems[i].MCount < need)
			{
				need -= InventoryItems[i].MCount;
				InventoryItems[i] = FInventoryItem();
			}
			else if (InventoryItems[i].MCount == need)
			{
				InventoryItems[i] = FInventoryItem();
				break;
			}
			else
			{
				InventoryItems[i].MCount -= need;
				break;
			}
		}
	}

}
// 2025.04.06 21:56 shs
void UMBFInventoryComponent::RequiredItemsCheck(
	TMap<FName, int32>& Map,			// Craft things, count
	TArray<FName>& RequiredCraftings,	// Have to Craft thigs
	TMap<int32, TPair<int32, int32>>& BasicMaterials, // Basic Material Map
	int32 ItemID,
	TPair<int32, int32> count
)
{

	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());
	const FItemData* Itemdata = Instance->GetItemData(FName(FString::FromInt(ItemID)));


	if (Itemdata->RequiredItems.Num() == 0)
	{
		BasicMaterials.FindOrAdd(ItemID).Key += count.Key;
		BasicMaterials.FindOrAdd(ItemID).Value += count.Value;
		return;
	}

	int32 CreateCount = FMath::Max(Itemdata->CreateCount, 1); //		 0방지
	int32 CraftCount = FMath::CeilToInt((float)count.Key / CreateCount);
	int32 VCraftCount = FMath::CeilToInt((float)count.Value / CreateCount);


	for (auto k : Itemdata->RequiredItems)
	{
		int32 FullNeeded = CraftCount * k.RequiredCount;
		int32 Needed = VCraftCount * k.RequiredCount;
		TPair<int32, int32> NextCount(FullNeeded, Needed);
		RequiredItemsCheck(Map, RequiredCraftings, BasicMaterials, k.ItemID, NextCount);

		FName Item = FName(FString::FromInt(k.ItemID));

		Map.FindOrAdd(Item) += Needed;

		if (!RequiredCraftings.Contains(Item))
		{
			RequiredCraftings.Add(Item);
		}

	}



}

//제작하는데 재료가 충분한지 검사
EMaterialCheckResult UMBFInventoryComponent::CheckMaterialAvailability(int32 ItemID, int32 RequiredCount)
{
	int Index = FindInventoryItem(ItemID);
	if (Index == -1)
	{
		return EMaterialCheckResult::NotFound;
	}

	if (GetInventoryItemCount(ItemID) >= RequiredCount)
	{
		return EMaterialCheckResult::Success;
	}
	else
	{
		return EMaterialCheckResult::NotEnough;
	}
}

int32 UMBFInventoryComponent::FindInventoryItem(int32 ItemID)
{
	for (int i = 0; i < 80; i++)
	{
		if (InventoryItems[i].ItemID == 0)
			continue;
		if (InventoryItems[i].ItemID == ItemID)
		{
			return i;
		}
	}
	return -1;
}

int32 UMBFInventoryComponent::GetInventoryItemCount(int32 ItemID)
{
	int32 Total = 0;
	for (auto& Item : InventoryItems)
	{
		if (Item.ItemID == ItemID)
		{
			Total += Item.MCount;
		}
	}
	return Total;

}

// On/Off Inventory
void UMBFInventoryComponent::InventoryTogle()
{
	if (!GetOwner()) return;

	UWorld* World = GetOwner()->GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return;

	AHUD* HUD = PC->GetHUD();
	AMBFHUD* MBFHUD = Cast<AMBFHUD>(HUD);
	if (!MBFHUD) return;

	if (bOpenInventory)
	{
		MBFHUD->CloseInventory();
		bOpenInventory = false;
	}
	else
	{
		MBFHUD->OpenInventory();
		bOpenInventory = true;
	}
}