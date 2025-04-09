// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameMenuWidget.generated.h"

class UButton;
class UReplayMenuWidget;

UCLASS()
class MBFACTORIO_API UGameMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

protected:
    // 클릭 시 호출될 함수들
    UFUNCTION()
    void OnClickContinue();

    UFUNCTION()
    void OnClickReplay();

    UFUNCTION()
    void OnClickExit();

protected:
    // 버튼을 바인딩할 변수들
    UPROPERTY(meta = (BindWidget))
    UButton* ButtonContinue;

    UPROPERTY(meta = (BindWidget))
    UButton* ButtonReplay;

    UPROPERTY(meta = (BindWidget))
    UButton* ButtonExit;

    UPROPERTY()
    UReplayMenuWidget* ReplayMenu;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UReplayMenuWidget> ReplayMenuClass;    
};
