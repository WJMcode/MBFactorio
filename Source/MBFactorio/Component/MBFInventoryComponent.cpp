// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MBFInventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Tools/MBFInstance.h"
#include "Tools/Widget/CraftList.h"
//#include "Struct/MBFStruct.h"
#include "Tools/MBFHUD.h"
#include "Math/UnrealMathUtility.h"
#include "Tools/MBFController.h"

#include "Character/PlayerCharacter.h"
#include "Tools/WJMController.h"

// Sets default values for this component's properties
UMBFInventoryComponent::UMBFInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	Inventory.SetNum(80);
	// Inventory[0] = FInventoryItem(FName("5"), 100, 100, EItemType::Smeltable);
	// Inventory[1] = FInventoryItem(FName("8"), 100, 5, EItemType::Smeltable);
	// ...
}


// Called when the game starts
void UMBFInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int i = 1; i < 17; i++)
	{
		BringItems.FindOrAdd(FName(FString::FromInt(i))) += GetInventoryItemCount(FName(FString::FromInt(i)));
	}
	// ...
	
}


// Called every frame
void UMBFInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	
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
			EItemType ItemType = Itemdata->ItemType;
			while (ItemCount != 0)
			{
				if (MaxCount <= ItemCount)
				{
					Inventory[Islot] = FInventoryItem(FName(FString::FromInt(i)), MaxCount, MaxCount, ItemType);
					ItemCount -= MaxCount;
				}
				else
				{
					Inventory[Islot] = FInventoryItem(FName(FString::FromInt(i)), MaxCount, ItemCount, ItemType);
					ItemCount = 0;
				}
				Islot += 1;
			}
		}

	}
	for (int i = Islot; i < 80; i++)
	{
		Inventory[i] = FInventoryItem();
	}
}

// Fill out your copyright notice in the Description page of Project Settings.






// 아이템 제작

void UMBFInventoryComponent::AddItem(FName ItemID, int32 Count)
{
	
	BringItems.FindOrAdd(ItemID) += Count;
	SortInventory();

	// 인벤토리 슬롯 UI 갱신
	AWJMController* PC = Cast<AWJMController>(GetOwner()->GetInstigatorController());
	if (PC->GetInventoryWidget())
	{
		UMBFInventory* InventoryWidget = Cast<UMBFInventory>(PC->GetInventoryWidget());
		InventoryWidget->UpdateInventoryUI();  // UI 갱신 함수 호출
	}
}
void UMBFInventoryComponent::RemoveItem(FName ItemID, int32 Count)
{
	
	BringItems.FindOrAdd(ItemID) -= Count;
	SortInventory();

	// 인벤토리 슬롯 UI 갱신
	AWJMController* PC = Cast<AWJMController>(GetOwner()->GetInstigatorController());
	if (PC->GetInventoryWidget())
	{
		UMBFInventory* InventoryWidget = Cast<UMBFInventory>(PC->GetInventoryWidget());
		InventoryWidget->UpdateInventoryUI();  // UI 갱신 함수 호출
	}
}
// 2025.04.06 21:56 shs

void UMBFInventoryComponent::DropItem()
{
	// 캐릭터의 전방 방향과 위치를 계산하여 아이템을 스폰
	APlayerCharacter* Character = Cast<APlayerCharacter>(GetOwner());
	if (Character)
	{
		FVector SpawnLocation = Character->GetActorLocation() - FVector(0, 0, Character->GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		SpawnLocation += Character->GetActorForwardVector() * 100.0f; // 앞쪽으로 좀 던지기
		SpawnLocation.Z += 10.f;
				
		SpawnItem(Inventory[0].ItemID, SpawnLocation);

		RemoveItem(Inventory[0].ItemID, 1);
	}

}

void UMBFInventoryComponent::SpawnItem(FName ItemID, FVector Location)
{
	if (ItemBlueprintMap.Contains(ItemID))
	{
		TSubclassOf<AActor> ItemClass = ItemBlueprintMap[ItemID];
		if (ItemClass)
		{
			FActorSpawnParameters Params;
			Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			GetWorld()->SpawnActor<AActor>(ItemClass, Location, FRotator::ZeroRotator, Params);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ItemBlueprintMap에 해당 ID (%s)가 없음"), *ItemID.ToString());
	}
}

int32 UMBFInventoryComponent::FindInventoryItem(FName ItemID)
{
	for (int i = 0; i < 80; i++)
	{
		if (Inventory[i].ItemID == FName("0"))
			continue;
		if (Inventory[i].ItemID == ItemID)
		{
			return i;
		}
	}
	return -1;
}

int32 UMBFInventoryComponent::GetInventoryItemCount(FName ItemID)
{
	int32 Total = 0;
	for (auto& Item : Inventory)
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

bool UMBFInventoryComponent::IsValidSlot(int32 SlotIndex) const
{
	return Inventory.IsValidIndex(SlotIndex);
}

void UMBFInventoryComponent::TrySwapOrMoveItems(
	int32 FromIndex, int32 ToIndex, UMBFInventoryComponent* FromInventory , FInventoryItem* InventoryItem)
{
	if (!IsValidSlot(ToIndex)) return;

	if (FromInventory == this)
	{
		// 같은 인벤토리 내에서 이동
		if (InventoryType == EInventoryType::Furnace)
		{
			return;
		}
		Swap(Inventory[FromIndex], Inventory[ToIndex]);
	}
	// 화로의경우 실패사유
	if (InventoryType == EInventoryType::Furnace)
	{
		if (ToIndex == 1)
		{
			if (InventoryItem->ItemType != EItemType::Fuel)
			{
				return;
			}
		}
		else if (ToIndex == 0)
		{
			if (!(InventoryItem->ItemType == EItemType::Smeltable || InventoryItem->ItemType == EItemType::SmeltableAndSmelted))
			{
				return;
			}
		}
	}
	else
	{
		// 다른 인벤토리 간 이동
		if (Inventory[ToIndex].IsEmpty())
		{
			
			// 그냥 옮기기
			Inventory[ToIndex] = FromInventory->Inventory[FromIndex];
			FromInventory->Inventory[FromIndex] = FInventoryItem();
		}
		else
		{
			// 교환
			Swap(Inventory[ToIndex], FromInventory->Inventory[FromIndex]);
		}
	}
	if (SortOption)
	{
		SortInventory();
	}
	if (FromInventory->SortOption)
	{
		FromInventory->SortInventory();
	}
}