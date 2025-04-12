// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MiningCompleteTextWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UMiningCompleteTextWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    UFUNCTION(BlueprintCallable)
    void SetMessageText(const FText& NewText);

protected:
    UPROPERTY(meta = (BindWidget))
    UTextBlock* MessageTextBlock;
};
