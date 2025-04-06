// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Struct/MBFStruct.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Tools/MBFHUD.h"
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


	UPROPERTY(EditAnywhere, Category = "Inventory Item")
	TArray<FInventoryItem> InventoryItems;


public:
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventoryItem GetInventoryItem(int32 b) { return InventoryItems[b]; }


protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	bool bOpenInventory;

	void InventoryTogle();


};
