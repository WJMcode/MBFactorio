// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/MBFInventoryComponent.h"
#include "Components/CapsuleComponent.h"
#include "Tools/MBFInstance.h"
#include "Tools/Widget/CraftList.h"
//#include "Struct/MBFStruct.h"
#include "Tools/MBFHUD.h"
#include "Tools/Widget/FurnaceInventory.h"
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

	for (int i = 0; i < 80; i++)
	{
		Inventory[i] = FInventoryItem();
	}

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

void UMBFInventoryComponent::SetBringItems()
{
	BringItems.Empty();

	for (int i = 1; i < 17; i++)
	{

		BringItems.FindOrAdd(FName(FString::FromInt(i))) += GetInventoryItemCount(FName(FString::FromInt(i)));
	}
}

void UMBFInventoryComponent::SetInventoryIndex(int32 InIndex, FName InItemID)
{
	if (Inventory[InIndex].ItemID == FName("0"))
	{
		FItemData* ItemData = Cast<UMBFInstance>(GetWorld()->GetFirstPlayerController()->GetGameInstance())->GetItemData(InItemID);
		if (ItemData) {
			Inventory[InIndex] = FInventoryItem(InItemID, ItemData->MaxCount, 0, ItemData->ItemType);
		}
	}
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
	//AWJMController* PC = Cast<AWJMController>(GetOwner()->GetInstigatorController());
	//if (PC->GetInventoryWidget())
	//{
	//	UMBFInventory* InventoryWidget = Cast<UMBFInventory>(PC->GetInventoryWidget());
	//	InventoryWidget->UpdateInventoryUI();  // UI 갱신 함수 호출
	//}
}
void UMBFInventoryComponent::RemoveItem(FName ItemID, int32 Count)
{
	
	BringItems.FindOrAdd(ItemID) -= Count;
	SortInventory();

	// 인벤토리 슬롯 UI 갱신
	//AWJMController* PC = Cast<AWJMController>(GetOwner()->GetInstigatorController());
	//if (PC->GetInventoryWidget())
	//{
	//	UMBFInventory* InventoryWidget = Cast<UMBFInventory>(PC->GetInventoryWidget());
	//	InventoryWidget->UpdateInventoryUI();  // UI 갱신 함수 호출
	//}
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


bool UMBFInventoryComponent::IsValidSlot(int32 SlotIndex) const
{
	return Inventory.IsValidIndex(SlotIndex);
}

void UMBFInventoryComponent::TrySwapOrMoveItems(
	int32 FromIndex, int32 ToIndex, UMBFInventoryComponent* FromInventory, FInventoryItem* InventoryItem)
{
	if (!IsValidSlot(ToIndex)) return;

	if (FromInventory == this)
	{
		return;
	}

	// ✅ CraftMachine 용 스택 처리
	if (InventoryType == EInventoryType::CraftMachine &&
		!Inventory[ToIndex].IsEmpty() &&
		Inventory[ToIndex].ItemID == FromInventory->Inventory[FromIndex].ItemID)
	{
		int32 MaxCount = Inventory[ToIndex].MaxCount;
		int32 CurrentCount = Inventory[ToIndex].MCount;
		int32 FromCount = FromInventory->Inventory[FromIndex].MCount;
		int32 AvailableSpace = MaxCount - CurrentCount;

		if (AvailableSpace > 0)
		{
			int32 MoveCount = FMath::Min(FromCount, AvailableSpace);

			Inventory[ToIndex].MCount += MoveCount;
			FromInventory->Inventory[FromIndex].MCount -= MoveCount;

			if (FromInventory->Inventory[FromIndex].MCount <= 0)
			{
				FromInventory->Inventory[FromIndex] = FInventoryItem();
			}
			FromInventory->SetBringItems();

			if (InventoryType == EInventoryType::Charactor)
				SortInventory();
			else if (FromInventory->InventoryType == EInventoryType::Charactor)
				FromInventory->SortInventory();
			ACraftMachine* CraftMachineOwner = Cast<AMBFController>(GetWorld()->GetFirstPlayerController())->GetCraftMachineOwner();
			CraftMachineOwner->GetAutoCraftUI()->OnChanged();
			FromInventory->SortInventory();
			return; // ✅ 여기서 끝
		}
	}

	// ✅ 기본 아이템 이동/교환 로직
	if (Inventory[ToIndex].IsEmpty())
	{
		if (CanMoveItem(FromIndex, ToIndex, FromInventory, InventoryItem))
		{
			Inventory[ToIndex] = FromInventory->Inventory[FromIndex];
			FromInventory->Inventory[FromIndex] = FInventoryItem();

			SetBringItems();
			FromInventory->SetBringItems();

			if (InventoryType == EInventoryType::Charactor)
				SortInventory();
			else if (FromInventory->InventoryType == EInventoryType::Charactor)
				FromInventory->SortInventory();

			// ✅ Furnace 조건 처리
			if (InventoryType == EInventoryType::Furnace || FromInventory->InventoryType == EInventoryType::Furnace)
			{
				ATestActor* FurnaceOwner = Cast<AMBFController>(GetWorld()->GetFirstPlayerController())->GetFurnaceOwner();
				if (FurnaceOwner)
				{
					FurnaceOwner->GetFurnaceUI()->FurNaceChagned();
					FurnaceOwner->GetFurnaceUI()->OnChanged();
				}
			}
		}
	}
	else
	{
		if (CanMoveItem(FromIndex, ToIndex, FromInventory, InventoryItem))
		{
			Swap(Inventory[ToIndex], FromInventory->Inventory[FromIndex]);

			SetBringItems();
			FromInventory->SetBringItems();

			if (InventoryType == EInventoryType::Charactor)
				SortInventory();
			else if (FromInventory->InventoryType == EInventoryType::Charactor)
				FromInventory->SortInventory();

			// ✅ Furnace 조건 처리
			if (InventoryType == EInventoryType::Furnace || FromInventory->InventoryType == EInventoryType::Furnace)
			{
				ATestActor* FurnaceOwner = Cast<AMBFController>(GetWorld()->GetFirstPlayerController())->GetFurnaceOwner();
				if (FurnaceOwner)
				{
					FurnaceOwner->GetFurnaceUI()->FurNaceChagned();
					FurnaceOwner->GetFurnaceUI()->OnChanged();
				}
			}
		}
	}
}
bool UMBFInventoryComponent::CanMoveItem(int32 FromIndex, int32 ToIndex, UMBFInventoryComponent* FromInventory, FInventoryItem* InventoryItem)
{
	// 이전에 클릭된 인벤토리 슬롯이 화로면
	if (FromInventory->InventoryType == EInventoryType::Furnace)
	{
		
		if (FromIndex == 0 && InventoryItem->ItemType == EItemType::Fuel)
		{
			return true;
		}
		else if (FromIndex == 1 && InventoryItem->ItemType == EItemType::Smeltable)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else if (InventoryType == EInventoryType::Furnace)
	{
		if (ToIndex == 0 && FromInventory->Inventory[FromIndex].ItemType == EItemType::Fuel)
		{
			return true;
		}
		else if (ToIndex == 1 && FromInventory->Inventory[FromIndex].ItemType == EItemType::Smeltable)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	
	return false;
}
