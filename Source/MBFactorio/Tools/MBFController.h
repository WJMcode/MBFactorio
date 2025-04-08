// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Struct/MBFStruct.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Tools/MBFHUD.h"
#include "Misc/Misc.h"
#include "Component/MBFInventoryComponent.h"
#include "MBFController.generated.h"

/**
 *
 */
UCLASS()
class MBFACTORIO_API AMBFController : public APlayerController
{
	GENERATED_BODY()


	UInputMappingContext* IMC_Default;

	AMBFController();
	
public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventoryItem GetInventoryItem(int32 b) { return MBFInventoryComponent->GetInventoryItem(b); }
	UMBFInventoryComponent* GetInventoryComponent() { return MBFInventoryComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:

	bool bOpenInventory;


	void CraftItem();
	void Crafting(int32 ItemID);


	int32 GetInventoryItemCount(int32 ItemID);											//인벤토리에 해당 아이템이 몇개 있는지 검사

	void InventoryTogle();

private:
	UMBFInventoryComponent* MBFInventoryComponent;


};
