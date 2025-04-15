// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools/Widget/MBFSlot.h"



class ACraftMachineTile;

#include "CraftMachineSlot.generated.h"

/**
 * 
 */

UCLASS()
class MBFACTORIO_API UCraftMachineSlot : public UMBFSlot
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);




private:


	UPROPERTY()
	ACraftMachineTile* OwnerTile;



public:
	void SetOwnerActor(ACraftMachineTile* InTile) { OwnerTile = InTile; }
	void InitSlot(int32 InSlot);
};
