// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tools/Widget/MBFSlot.h"
#include "Struct/MBFStruct.h"
#include "Tools/Widget/CraftSelectSlot.h"
#include "Tools/Widget/CraftSlot.h"
#include "Components/ProgressBar.h"
#include "FurnaceInventory.generated.h"

/**
 *
 */
UCLASS()
class MBFACTORIO_API UFurnaceInventory : public UUserWidget
{
	GENERATED_BODY()


	UPROPERTY()
	UMBFSlot* ItemSlot[80];
	UPROPERTY()
	UMBFSlot* FurnaceSlot[3];
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* ProgressBar;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UProgressBar* FuelBar;

	UPROPERTY()
	UMBFInventoryComponent* OwnerInventory;
	UPROPERTY()
	UMBFInventoryComponent* FurnaceInventory;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UMBFSlot* GetItemSlot(int32 b) { return ItemSlot[b]; }

	void SlotChanged(int32 InSlot) { ItemSlot[InSlot]->Changed(InSlot); }
	void FurNaceChagned( ); 
	void SetPercent(float Progresspercent, float Fuelpercent);

	void SetOwnerInventory(UMBFInventoryComponent* InFurnaceInventory);


	void OnChanged();


protected:



	void OnInitialized();
	void NativeConstruct() override;
};


