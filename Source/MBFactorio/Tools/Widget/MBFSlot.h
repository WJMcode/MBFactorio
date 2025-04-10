// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "MBFSlot.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API UMBFSlot : public UUserWidget
{
	GENERATED_BODY()



	
protected:
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Back;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UImage* Image;
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UTextBlock* Count;
	UBorder* LRBorder;
	UBorder* UpBorder;
	UBorder* DownBorder;
	

	FName ItemID;

protected:
	virtual void NativeConstruct() override;


public:

	UButton* MyButton;
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UImage* GetImage() { return Image; }
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	UTextBlock* GetTextBlock() { return Count; }
	void SetItemID(FName InItemID) { ItemID = InItemID; }

public:
	
	void Changed(int32 InSlot);

};
