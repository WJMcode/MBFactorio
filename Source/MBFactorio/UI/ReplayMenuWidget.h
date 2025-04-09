// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/GameMenuWidget.h"
#include "ReplayMenuWidget.generated.h"

class UButton;

UCLASS()
class MBFACTORIO_API UReplayMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
    virtual void NativeConstruct() override;

protected:
    // 버튼을 바인딩할 변수들
    UPROPERTY(meta = (BindWidget))
    UButton* ButtonYes;

    UPROPERTY(meta = (BindWidget))
    UButton* ButtonNo;

    // 클릭 시 호출될 함수들
    UFUNCTION()
    void OnClickYes();

    UFUNCTION()
    void OnClickNo();
};
