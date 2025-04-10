﻿// Fill out your copyright notice in the Description page of Project Settings.

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
	TMap<FName, int32>			BringItems;
	TMap<FName, int32>			AfterChanged;


	FItemData* BuildItem;
	float BuildTime;
	float ElapsedCraftingTime;
	

public:	
	// Sets default values for this component's properties
	UMBFInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SortInventory();

	bool bOpenInventory;

	bool CanCraftItem(FName ItemID, int32 CraftCount, TMap<FName, int32>* OutMap = nullptr, TArray<FName>* OutRequiredCraftings = nullptr, TMap<FName, int32>* OutChnaged = nullptr);
	void CraftItem(FName ItemID, int32 CraftCount);
	void Crafting(FName ItemID);

	void AddItem(FName ItemID, int32 Count);
	void RemoveItem(FName ItemID, int32 Count);

	bool RequiredItemsCheck(TMap<FName, int32>& Map, TArray<FName>& RequiredCraftings, TMap<FName, int32>& ChangedItems, FName ItemID, int32 count);



	EMaterialCheckResult CheckMaterialAvailability(FName ItemID, int32 RequiredCount);	//제작하는데 재료가 충분한지 검사

	int32 FindInventoryItem(FName ItemID);												//인벤토리에 해당 아이템이 있는지 검사
	int32 GetInventoryItemCount(FName ItemID);
	FInventoryItem GetInventoryItem(int32 b) { return InventoryItems[b]; }
	const TArray<TPair<FName, int32>> GetCraftings() { return Craftings; }
		
	void InventoryTogle();
};
