// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tools/Widget/MBFSlot.h"
#include "Struct/MBFStruct.h"
#include "Tools/Widget/CraftSelectSlot.h"
#include "Tools/Widget/CraftSlot.h"
#include "MBFInventory.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UMBFInventory : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY()

	UMBFSlot* ItemSlot[80];
	UPROPERTY()
	UCraftSlot* CraftSlot[70];
	UPROPERTY()
	UCraftSelectSlot* SelectedSlot[4];
	int32 SelectedSlotNum;
	UPROPERTY()
	UMBFInventoryComponent* OwnerInventory;
public:
	void UpdateInventoryUI();

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UMBFSlot* GetItemSlot(int32 b) { return ItemSlot[b]; }
	void OnChanged();
	void SlotChanged(int32 InSlot) {ItemSlot[InSlot]->Changed(InSlot);}
	
	void BindingSelectedAction(UCraftSelectSlot* InSlot);
protected:
	UFUNCTION()
	void SelectedSlotChange(int32 SlotID);


	void OnInitialized();
	void NativeConstruct() override;
	
};
