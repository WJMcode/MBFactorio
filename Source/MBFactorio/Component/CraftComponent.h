// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Misc/Misc.h"
#include "Struct/MBFStruct.h"
#include "Component/MBFInventoryComponent.h"
#include "CraftComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MBFACTORIO_API UCraftComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCraftComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void SetInventoryComponent(UMBFInventoryComponent* InInventoryComponent) { InventoryComponent = InInventoryComponent; }


	bool CanCraftItem(FName ItemID, int32 CraftCount, TMap<FName, int32>* OutMap = nullptr, TArray<FName>* OutRequiredCraftings = nullptr, TMap<FName, int32>* OutChnaged = nullptr);
	EMaterialCheckResult CheckMaterialAvailability(FName ItemID, int32 RequiredCount);	//제작하는데 재료가 충분한지 검사
	bool RequiredItemsCheck(TMap<FName, int32>& Map, TArray<FName>& RequiredCraftings, TMap<FName, int32>& ChangedItems, FName ItemID, int32 count);
	void CraftItem(FName ItemID, int32 CraftCount);
	void Crafting();
	

	TArray<TPair<FName, int32>> GetCraftings() { return Craftings; }

private:
	TArray<TPair<FName, int32>> Craftings;
	TMap<FName, int32>			AfterChanged;
	FItemData* BuildItem;
	float BuildTime;
	float mBuildTime;


	UPROPERTY()
	UMBFInventoryComponent* InventoryComponent;
	
	// 조합기계인지 체크
	UPROPERTY()
	bool bMachine;

	// 조합기계 레벨 체크(아이템 재료개수제한 증가)
	UPROPERTY()
	int32 MachineLevel;

};
