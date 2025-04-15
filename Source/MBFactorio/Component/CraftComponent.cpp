// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/CraftComponent.h"
#include "Tools/MBFInstance.h"
#include "Component/MBFInventoryComponent.h"
#include "Tools/MBFHUD.h"
#include "Tools/MBFController.h"
#include "Misc/Misc.h"

// Sets default values for this component's properties
UCraftComponent::UCraftComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UCraftComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UCraftComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (Craftings.Num())
	{
		if (BuildItem == nullptr)
		{
			BuildItem = Cast<UMBFInstance>(GetWorld()->GetGameInstance())->GetItemData(Craftings[0].Key);
			BuildTime = BuildItem->BuildTime;
			mBuildTime = 0;
		}
		else
		{
			AMBFHUD* HUD = Cast<AMBFHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());
			HUD->GetCraftListUI()->DeltaChange(mBuildTime / BuildTime);
			mBuildTime += DeltaTime;
			if (BuildTime < mBuildTime)
			{
				mBuildTime -= BuildTime;
				Craftings[0].Value -= 1;

				Crafting();
				if (Craftings[0].Value == 0)
				{
					BuildItem = nullptr;
					Craftings.RemoveAt(0);
					BuildTime = 0;
					HUD->GetCraftListUI()->DeltaChange(0);
				}

				HUD->OnChanged();
				HUD->GetCraftListUI()->CraftChange();
			}
		}
	}
}

bool UCraftComponent::CanCraftItem(FName ItemID, int32 CraftCount, TMap<FName, int32>* OutMap, TArray<FName>* OutRequiredCraftings, TMap<FName, int32>* OutChanged)
{

	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());
	const FItemData* Itemdata = Instance->GetItemData(ItemID);

	if (!Itemdata || !Itemdata->bMake) { return false; }

	TMap<FName, int32> LocalMap;
	TArray<FName> LocalRequiredCraftings;
	TMap<FName, int32> LocalChanged;
	TMap<FName, TPair<int32, int32>> BasicMaterials;



	TMap<FName, int32>& MapRef = OutMap ? *OutMap : LocalMap;
	TArray<FName>& CraftingsRef = OutRequiredCraftings ? *OutRequiredCraftings : LocalRequiredCraftings;
	TMap<FName, int32>& ChangedRef = OutChanged ? *OutChanged : LocalChanged;


	if (!RequiredItemsCheck(MapRef, CraftingsRef, ChangedRef, ItemID, CraftCount*Itemdata->CreateCount))
	{
		return false;
	}

	for (auto& BasicMaterial : BasicMaterials)
	{
		if (CheckMaterialAvailability(BasicMaterial.Key, BasicMaterial.Value.Value) != EMaterialCheckResult::Success)
			return false;
		// 경고 문구 추가시 요기에
	}


	MapRef.FindOrAdd(ItemID) += CraftCount;
	CraftingsRef.Add(ItemID);
	return true;


}
EMaterialCheckResult UCraftComponent::CheckMaterialAvailability(FName ItemID, int32 RequiredCount)
{
	int Index = InventoryComponent->FindInventoryItem(ItemID);
	if (Index == -1)
	{
		return EMaterialCheckResult::NotFound;
	}
	else if (int32 pkp = *(AfterChanged.Find(ItemID)))
	{
		if ((*InventoryComponent->BringItems.Find(ItemID) + pkp >= RequiredCount))
		{
			return EMaterialCheckResult::Success;
		}
		return EMaterialCheckResult::NotEnough;
	}
	else if (*(InventoryComponent->BringItems.Find(ItemID)) >= RequiredCount)
	{
		return EMaterialCheckResult::Success;
	}
	else
	{
		return EMaterialCheckResult::NotEnough;
	}
}
bool UCraftComponent::RequiredItemsCheck(TMap<FName, int32>& Map, TArray<FName>& RequiredCraftings, TMap<FName, int32>& ChangedItems, FName ItemID, int32 count)
{
	if (count <= 0) {
		return true;
	}
	
	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());

	const FItemData* Itemdata = Instance->GetItemData(ItemID);
	//changedItems에 바로 추가 만들어지는 아이템
	{
		float CraftCount = (float)count / Itemdata->CreateCount;
		ChangedItems.FindOrAdd(ItemID) += FMath::CeilToInt(CraftCount)*Itemdata->CreateCount;
	}
	if (Itemdata->RequiredItems.Num() == 0)
	{
		int k = AfterChanged.Find(ItemID) ? *AfterChanged.Find(ItemID) : 0;
		if ((*(InventoryComponent->BringItems.Find(ItemID)) + k) < count)
		{
			return false;
		}
	}
	for (auto& RequiredItem : Itemdata->RequiredItems)	//electronic
	{
		
		float ratio = static_cast<float>(count) / FMath::Max(Itemdata->CreateCount, 1);	// cable ratio = 1
		int32 needCount = FMath::CeilToInt(ratio * RequiredItem.RequiredCount);			// cable needCount = 15
		
		
	
		FName ID = RequiredItem.ItemID;
		ChangedItems.FindOrAdd(ID) -= needCount;
		if (InventoryComponent->BringItems.Find(ID) != nullptr)
		{
			needCount -= (*(InventoryComponent->BringItems.Find(ID)));						// cable needCount = 15
		}
		if (AfterChanged.Find(ID) != nullptr)
		{
			needCount -= *(AfterChanged.Find(ID));
		}
		if (needCount > 0) {

			FItemData* RequiredItemData = Instance->GetItemData(RequiredItem.ItemID);
			if (RequiredItemData->ItemType == EItemType::Smeltable || RequiredItemData->ItemType == EItemType::SmeltableAndSmelted)
			{
				return false;
			}

			if (!RequiredItemsCheck(Map, RequiredCraftings, ChangedItems, RequiredItem.ItemID, needCount))
			{
				return false;
			}

			FItemData* data = Instance->GetItemData(RequiredItem.ItemID);
			int32 CraftCount = FMath::CeilToInt(static_cast<float>(needCount) / data->CreateCount);
			Map.FindOrAdd(ID) += CraftCount;
			// 재료아이템 감산연산



			RequiredCraftings.Add(ID);
		}
		
	}

	return true;
}
void UCraftComponent::CraftItem(FName ItemID, int32 CraftCount)
{
	UMBFInstance* Instance = Cast<UMBFInstance>(GetWorld()->GetGameInstance());
	const FItemData* ItemData = Instance->GetItemData(ItemID);
	if (ItemData == nullptr)
	{
		return;
	}
	TMap<FName, int32> Map;
	TArray<FName> RequiredItems;
	TMap<FName, int32> Changed;
	InventoryComponent->SetBringItems();
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
	HUD->GetCraftListUI()->CraftChange();
}
// RequiredItemCheck이후
void UCraftComponent::Crafting()
{
	TArray<FRequiredItem> RequiredItems = BuildItem->RequiredItems;

	for (auto RequiredItem : RequiredItems)
	{
		InventoryComponent->RemoveItem(RequiredItem.ItemID, RequiredItem.RequiredCount);
		AfterChanged.FindOrAdd(RequiredItem.ItemID) += RequiredItem.RequiredCount;
	}
	InventoryComponent->AddItem(BuildItem->ItemID, BuildItem->CreateCount);
	AfterChanged.FindOrAdd(BuildItem->ItemID) -= BuildItem->CreateCount;
}