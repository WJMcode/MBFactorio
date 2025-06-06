﻿#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Struct/MBFStruct.h"
#include "Misc/Misc.h"
#include "MBFInventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MBFACTORIO_API UMBFInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TMap<FName, TSubclassOf<AActor>> ItemBlueprintMap;

	TMap<FName, int32>			BringItems;
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FInventoryItem* GetInventoryIndex(int32 InSlot) { return &Inventory[InSlot]; }

	void SortInventory();

	void SetBringItems();

	void SetInventoryIndex(int32 InIndex,FName InItemID);

	bool bOpenInventory;

	void AddItem(FName ItemID, int32 Count);
	void RemoveItem(FName ItemID, int32 Count);

public:
	void DropItem();
	void SpawnItem(FName ItemID, FVector Location);
		
	int32 FindInventoryItem(FName ItemID);												//인벤토리에 해당 아이템이 있는지 검사
	int32 GetInventoryItemCount(FName ItemID);
	FInventoryItem& GetInventoryItem(int32 b) { return Inventory[b]; }
		

	void SetInventoryNum(int32 Num) { Inventory.SetNum(Num); }

	void SetSortOption(bool InSortOption) { SortOption = InSortOption; }

	void SetInventoryType(EInventoryType InInventoryType) { InventoryType = InInventoryType; }

	bool IsValidSlot(int32 SlotIndex) const;
	void TrySwapOrMoveItems(int32 FromIndex, int32 ToIndex, UMBFInventoryComponent* FromInventory, FInventoryItem* InventoryItem);

	bool CanMoveItem(int32 FromIndex, int32 ToIndex, UMBFInventoryComponent* FromInventory, FInventoryItem* InventoryItem);


};
