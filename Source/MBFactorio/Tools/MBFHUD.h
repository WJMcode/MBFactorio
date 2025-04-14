// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Tools/Widget/MBFInventory.h"
#include "Tools/Widget/CraftList.h"
#include "Tools/Widget/FurnaceInventory.h"
#include "MBFHUD.generated.h"

/**
 * 
 */
class UMiningInteractionWidget;

UCLASS()
class MBFACTORIO_API AMBFHUD : public AHUD
{
	GENERATED_BODY()
	
	




	AMBFHUD();
	virtual void BeginPlay() override;
	
private:

	UFUNCTION()
	void UpdateMiningProgress(float Progress);

	// UMiningInteractionWidget의 CompleteText를 출력합니다.
	UFUNCTION()
	void ShowMiningComplete(EResourceType Resource);

	// 채굴 중지 시 ProgressBar를 숨깁니다.
	UFUNCTION()
	void HideMiningProgress();


public:

	void OpenInventory();
	void CloseInventory();

	void OnChanged() { if(Inventory != nullptr)Cast<UMBFInventory>(Inventory)->OnChanged(); }
	void SlotChanged(int32 InSlot);


	UCraftList* GetCraftListUI();


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMiningInteractionWidget> MiningInteractionWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UMiningInteractionWidget* MiningInteractionWidget;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UMBFInventory* Inventory;
	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UCraftList* CraftingUI;
};
