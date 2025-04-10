// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tools/Widget/CraftingSlot.h"
#include "CraftList.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UCraftList : public UUserWidget
{
	GENERATED_BODY()
	UCraftingSlot* CraftSlots[10];

	
protected:

	virtual void NativeConstruct() override;

	

public:
	void CraftChange();
	void DeltaChange(float Percent);
};
