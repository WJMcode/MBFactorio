// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Struct/MBFStruct.h"
#include "MBFController.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API AMBFController : public APlayerController
{
	GENERATED_BODY()


	AMBFController();


	UPROPERTY(EditAnywhere, Category = "Inventory Item")
	TArray<FInventoryItem> InventoryItems;


protected:
	virtual void BeginPlay() override;

};
