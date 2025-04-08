// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tools/Widget/MBFSlot.h"
#include "Struct/MBFStruct.h"

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

public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UMBFSlot* GetItemSlot(int32 b) { return ItemSlot[b]; }
	void OnChanged();

private:
	
	void OnInitialized();
	void NativeConstruct() override;
	
};
