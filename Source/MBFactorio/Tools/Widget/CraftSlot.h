// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools/Widget/MBFSlot.h"
#include "Components/Button.h"
#include "CraftSlot.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UCraftSlot : public UMBFSlot
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
public:
	void CraftChange();
};
