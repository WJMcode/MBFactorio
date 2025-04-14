// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MBFGamemode.generated.h"

/**
 * 
 */
UCLASS()
class MBFACTORIO_API AMBFGamemode : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	AMBFGamemode();

    virtual void BeginPlay() override;

    // UI 블루프린트 클래스
    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<class UMBFStartWidget> StartWidgetClass;

    // 실제 생성된 위젯
    UPROPERTY()
    class UMBFStartWidget* StartWidget;
};
