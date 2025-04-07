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
	TArray<TPair<FName, int32>> Craftings;

public:	
	// Sets default values for this component's properties
	UMBFInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool bOpenInventory;

	bool CanCraftItem(int32 ItemID, int32 CraftCount, TMap<FName, int32>* OutMap = nullptr, TArray<FName>* OutRequiredCraftings = nullptr);
	void CraftItem(int32 ItemID, int32 CraftCount);
	void Crafting(int32 ItemID);

	void AddItem(int32 ItemID, int32 Count);
	void RemoveItem(int32 ItemID, int32 Count);

	void RequiredItemsCheck(TMap<FName, int32>& Map, TArray<FName>& RequiredCraftings, TMap<int32, TPair<int32, int32>>& BasicMaterials, int32 ItemID, TPair<int32, int32> count);


	EMaterialCheckResult CheckMaterialAvailability(int32 ItemID, int32 RequiredCount);	//제작하는데 재료가 충분한지 검사

	int32 FindInventoryItem(int32 ItemID);												//인벤토리에 해당 아이템이 있는지 검사
	int32 GetInventoryItemCount(int32 ItemID);
	FInventoryItem GetInventoryItem(int32 b) { return InventoryItems[b]; }
		
	void InventoryTogle();
};
