// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools/Widget/MBFSlot.h"





#include "CraftMachineSlot.generated.h"

/**
 * 
 */

class ACraftMachine;

UCLASS()
class MBFACTORIO_API UCraftMachineSlot : public UMBFSlot
{
	GENERATED_BODY()

	virtual void NativeConstruct() override;
	
	

	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);




private:


	UPROPERTY()
	ACraftMachine* OwnerActor;



public:
	void SetOwnerActor(ACraftMachine* InActor) { OwnerActor = InActor; }
	void InitSlot(int32 InSlot);
};
