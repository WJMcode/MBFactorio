// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools/Widget/MBFSlot.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "SelectedSlot.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotClicked, int32, SlotID);


UCLASS()
class MBFACTORIO_API USelectedSlot : public UMBFSlot
{
	
	
	GENERATED_BODY()
	
	UBorder* DownBorder;
	UBorder* UpBorder;
	UBorder* LWBorder;
	int32 SlotID;

protected:
	virtual void NativeConstruct() override;


public:
	UPROPERTY(BlueprintAssignable, Category = "Event")
	FOnSlotClicked OnSlotClicked;

	void SetNum(int32 InNum) { SlotID = InNum; }
	UFUNCTION()
	void Selected();
	void UnSelected();



};
