// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tools/Widget/CraftSlot.h"
#include "Craftings.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UCraftings : public UUserWidget
{
	GENERATED_BODY()
	UCraftSlot* CraftSlots[10];

	
protected:

	virtual void NativeConstruct() override;

	

public:
	void CraftChange();
	void DeltaChange(float Percent);
};
