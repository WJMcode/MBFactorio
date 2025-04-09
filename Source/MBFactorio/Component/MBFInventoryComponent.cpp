// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MBFInventoryComponent.h"
#include "Tools/MBFInstance.h"
#include "Tools/Widget/Craftings.h"
#include "Struct/MBFStruct.h"
#include "Tools/MBFHUD.h"
#include "Math/UnrealMathUtility.h"
#include "Tools/MBFController.h"

// Sets default values for this component's properties
UMBFInventoryComponent::UMBFInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	InventoryItems.SetNum(80);	
	InventoryItems[0] = FInventoryItem(5, 100, 100);
	InventoryItems[1] = FInventoryItem(8, 100, 100);
	// ...
}


// Called when the game starts
void UMBFInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 1; i < 17; i++)
	{
		BringItems.FindOrAdd(FName(FString::FromInt(i))) += GetInventoryItemCount(i);
		AfterChanged.Add(FName(FString::FromInt(i)));
	}
	// ...
	
}


// Called every frame
void UMBFInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Craftings.Num())
	{
		if (BuildItem == nullptr)
		{
			BuildItem = Cast<UMBFInstance>(GetWorld()->GetGameInstance())->GetItemData(Craftings[0].Key);
			BuildTime = BuildItem->BuildTime;
			ElapsedCraftingTime = 0;
		}
		else
		{
			AMBFHUD* HUD = Cast<AMBFHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			HUD->GetCraftingUI()->DeltaChange(ElapsedCraftingTime / BuildTime);
			ElapsedCraftingTime += DeltaTime;
			if (BuildTime < ElapsedCraftingTime)
			{
				ElapsedCraftingTime -= BuildTime;
				Craftings[0].Value -= 1;
				
				for (auto& RequiredItem : BuildItem->RequiredItems)
				{
					RemoveItem(RequiredItem.ItemID, RequiredItem.RequiredCount);
					AfterChanged.FindOrAdd(FName(FString::FromInt(RequiredItem.ItemID))) += RequiredItem.RequiredCount;
				}
				AddItem(BuildItem->ItemID, BuildItem->CreateCount);
				AfterChanged.FindOrAdd(FName(FString::FromInt(BuildItem->ItemID))) -= BuildItem->CreateCount;
				if (Craftings[0].Value == 0)
				{
					BuildItem = nullptr;
					Craftings.RemoveAt(0);
					BuildTime = 0;
					HUD->GetCraftingUI()->DeltaChange(0);
				}
				
				HUD->OnChanged();
				HUD->GetCraftingUI()->CraftChange();
			}
		}
	}
	// ...
}

void UMBFInventoryComponent::SortInventory()
{
	int32 Islot = 0;
	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());

	for (int i = 1; i < 17; i++)
	{
		FName ItemID = FName(FString::FromInt(i));
		int32 ItemCount = *(BringItems.Find(ItemID));
		if (0 < ItemCount)
		{
			FItemData* Itemdata = Instance->GetItemData(ItemID);
			int32 MaxCount = Itemdata->MaxCount;
			while (ItemCount != 0)
			{
				if (MaxCount <= ItemCount)
				{
					InventoryItems[Islot] = FInventoryItem(i, MaxCount, MaxCount);
					ItemCount -= MaxCount;
				}
				else
				{
					InventoryItems[Islot] = FInventoryItem(i, MaxCount, ItemCount);
					ItemCount = 0;
				}
				Islot += 1;
			}
		}

	}
	for (int i = Islot; i < 80; i++)
	{
		InventoryItems[i] = FInventoryItem();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.






// 아이템 제작
bool UMBFInventoryComponent::CanCraftItem(int32 ItemID, int32 CraftCount, TMap<FName, int32>* OutMap, TArray<FName>* OutRequiredCraftings, TMap<FName, int32>* OutChanged)
{
	
	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());
	const FItemData* Itemdata = Instance->GetItemData(FName(FString::FromInt(ItemID)));

	if (!Itemdata || !Itemdata->bMake) { return false; }

	TMap<FName, int32> LocalMap;
	TArray<FName> LocalRequiredCraftings;
	TMap<FName, int32> LocalChanged;
	TMap<int32, TPair<int32, int32>> BasicMaterials;



	TMap<FName, int32>& MapRef = OutMap ? *OutMap : LocalMap;
	TArray<FName>& CraftingsRef = OutRequiredCraftings ? *OutRequiredCraftings : LocalRequiredCraftings;
	TMap<FName, int32>& ChangedRef = OutChanged ? *OutChanged : LocalChanged;
	

	RequiredItemsCheck(MapRef, CraftingsRef, ChangedRef, ItemID, CraftCount);

	for (auto& BasicMaterial : BasicMaterials)
	{
		if (CheckMaterialAvailability(BasicMaterial.Key, BasicMaterial.Value.Value) != EMaterialCheckResult::Success)
			return false;
		// 경고 문구 추가시 요기에
	}
	MapRef.FindOrAdd(FName(FString::FromInt(ItemID))) += CraftCount;
	CraftingsRef.Add(FName(FString::FromInt(ItemID)));
	return true;


}
void UMBFInventoryComponent::CraftItem(int32 ItemID, int32 CraftCount)
{
	TMap<FName, int32> Map;
	TArray<FName> RequiredItems;
	TMap<FName, int32> Changed;
	if (CanCraftItem(ItemID, CraftCount, &Map, &RequiredItems, &Changed))
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
		for (int i = 1; i < 17; i++)
		{
			FName NameItem = FName(FString::FromInt(i));
			if (Changed.Find(NameItem) != nullptr)
			{
				AfterChanged.FindOrAdd(NameItem) += *(Changed.Find(NameItem));
			}
		}
		
	}
	AMBFHUD* HUD = Cast<AMBFHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
	HUD->GetCraftingUI()->CraftChange();
}
// RequiredItemCheck이후
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
	FName ItemIDName = FName(FString::FromInt(ItemID));
	BringItems.FindOrAdd(ItemIDName) += Count;
	SortInventory();
}
void UMBFInventoryComponent::RemoveItem(int32 ItemID, int32 Count)
{
	FName ItemIDName = FName(FString::FromInt(ItemID));
	BringItems.FindOrAdd(ItemIDName) -= Count;
	SortInventory();
}
// 2025.04.06 21:56 shs
void UMBFInventoryComponent::RequiredItemsCheck(
	TMap<FName, int32>& Map,			// Craft things, count
	TArray<FName>& RequiredCraftings,	// Have to Craft thigs
	TMap<FName, int32>& ChangedItems,
	int32 ItemID,
	int32 count
)
{
	if (count <= 0) {
		return;
	}
	
	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());
	FName RowName = FName(FString::FromInt(ItemID));

	const FItemData* Itemdata = Instance->GetItemData(RowName);
	//changedItems에 바로 추가 만들어지는 아이템
	{
		float CraftCount = (float)count / Itemdata->CreateCount;
		ChangedItems.FindOrAdd(RowName) += FMath::CeilToInt(CraftCount)*Itemdata->CreateCount;
	}
	for (auto& RequiredItem : Itemdata->RequiredItems)	//electronic
	{
		float ratio = static_cast<float>(count) / FMath::Max(Itemdata->CreateCount, 1);	// cable ratio = 1
		int32 needCount = FMath::CeilToInt(ratio * RequiredItem.RequiredCount);			// cable needCount = 15
		
		
	
		FName ID = FName(FString::FromInt(RequiredItem.ItemID));
		FName ItemName = FName(FString::FromInt(RequiredItem.ItemID));
		ChangedItems.FindOrAdd(ID) -= needCount;
		if (BringItems.Find(ItemName) != nullptr)
		{
			needCount -= (*(BringItems.Find(ItemName)));						// cable needCount = 15
		}
		if (AfterChanged.Find(ItemName) != nullptr)
		{
			needCount -= *(AfterChanged.Find(ItemName));
		}
		if (needCount <= 0) { return; }
		RequiredItemsCheck(Map, RequiredCraftings, ChangedItems, RequiredItem.ItemID, needCount);
	
		FItemData* data = Instance->GetItemData(FName(FString::FromInt(RequiredItem.ItemID)));
		int32 CraftCount = FMath::CeilToInt(static_cast<float>(needCount) / data->CreateCount);
		Map.FindOrAdd(ID) += CraftCount;
		// 재료아이템 감산연산

		

		RequiredCraftings.Add(ID);
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

	if (*(BringItems.Find(FName(FString::FromInt(ItemID)))) >= RequiredCount)
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