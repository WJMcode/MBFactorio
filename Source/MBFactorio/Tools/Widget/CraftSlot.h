// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tools/Widget/MBFSlot.h"
#include "Components/ProgressBar.h"
#include "CraftSlot.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UCraftSlot : public UMBFSlot
{
	GENERATED_BODY()
protected:

	UProgressBar* Progress;

	virtual void NativeConstruct() override;

public:
	void SlotChange(FName InItem = FName(), int32 InCount = 0);
	void ProgressChanged(float l = 0.0);


};
