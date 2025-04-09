// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tools/Widget/MBFSlot.h"
#include "Struct/MBFStruct.h"
#include "Tools/Widget/SelectedSlot.h"
#include "MBFInventory.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UMBFInventory : public UUserWidget
{
	GENERATED_BODY()
	

	UPROPERTY(EditAnywhere
	)

	UMBFSlot* ItemSlot[80];
	USelectedSlot* SelectedSlot[4];
	int32 SelectedSlotNum;

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UMBFSlot* GetItemSlot(int32 b) { return ItemSlot[b]; }
	void OnChanged();
	void SlotChanged(int32 i);
	
	void BindingSelectedAction(USelectedSlot* InSlot);
protected:
	UFUNCTION()
	void SelectedSlotChange(int32 SlotID);


	void OnInitialized();
	void NativeConstruct() override;
	
};
