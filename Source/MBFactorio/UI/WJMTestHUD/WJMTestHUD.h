// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Tiles/TileBase/TileStructs.h"
#include "WJMTestHUD.generated.h"

class UMiningInteractionWidget;

/**
 * UMiningInteractionWidget을 관리하고 있습니다.
 */
UCLASS()
class MBFACTORIO_API AWJMTestHUD : public AHUD
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

private:
	// UMiningInteractionWidget의 ProgressBar를 업데이트합니다.
	UFUNCTION()
	void UpdateMiningProgress(float Progress);
	
	// UMiningInteractionWidget의 CompleteText를 출력합니다.
	UFUNCTION()
	void ShowMiningComplete(EResourceType Resource);
	
	// 채굴 중지 시 ProgressBar를 숨깁니다.
	UFUNCTION()
	void HideMiningProgress();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UMiningInteractionWidget> MiningInteractionWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "UI")
	UMiningInteractionWidget* MiningInteractionWidget;
};
