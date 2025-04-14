// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools/Widget/MBFSlot.h"
#include "Components/ProgressBar.h"
#include "CraftingSlot.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UCraftingSlot : public UMBFSlot
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UProgressBar* Progress;

	virtual void NativeConstruct() override;

public:
	void SlotChange(FName InItem = FName(), int32 InCount = 0);
	void ProgressChanged(float l = 0.0);


};
