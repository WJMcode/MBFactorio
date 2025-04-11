// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/MBFStruct.h"
#include "Misc/Misc.h"
#include "MBFInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MBFACTORIO_API UMBFInventoryComponent : public UActorComponent
{
	GENERATED_BODY()


	TArray<FInventoryItem> InventoryItems;

	// 다른 컴포넌트에서 봣을 때 가독성이 좋은 이름
	TArray<FInventoryItem> Inventory;


	bool SortOption = false;
	EInventoryType InventoryType;

public:	
	// Sets default values for this component's properties
	UMBFInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	

	TMap<FName, int32>			BringItems;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SortInventory();

	bool bOpenInventory;


	void AddItem(FName ItemID, int32 Count);
	void RemoveItem(FName ItemID, int32 Count);

	

	int32 FindInventoryItem(FName ItemID);												//인벤토리에 해당 아이템이 있는지 검사
	int32 GetInventoryItemCount(FName ItemID);
	FInventoryItem GetInventoryItem(int32 b) { return InventoryItems[b]; }
		
	void InventoryTogle();

	void SetInventoryNum(int32 Num) { Inventory.SetNum(Num); }

	void SetSortOption(bool InSortOption) { SortOption = InSortOption; }

	void SetInventoryType(EInventoryType InInventoryType) { InventoryType = InInventoryType; }

	bool IsValidSlot(int32 SlotIndex) const;
	void TrySwapOrMoveItems(int32 FromIndex, int32 ToIndex, UMBFInventoryComponent* FromInventory, FInventoryItem* InventoryItem);
};
