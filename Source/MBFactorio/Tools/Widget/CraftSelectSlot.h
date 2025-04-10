// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools/Widget/MBFSlot.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "CraftSelectSlot.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, int32, SlotID);


UCLASS()
class MBFACTORIO_API UCraftSelectSlot : public UMBFSlot
{
	
	
	GENERATED_BODY()
	

	int32 SlotID;

protected:
	virtual void NativeConstruct() override;


public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnSlotClicked OnSlotClicked;

	void SetNum(int32 InNum);
	UFUNCTION()
	void Selected();
	void UnSelected();



};
