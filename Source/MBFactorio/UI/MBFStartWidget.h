// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MBFStartWidget.generated.h"

class UButton;

UCLASS()
class MBFACTORIO_API UMBFStartWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

    // 버튼을 바인딩할 변수들
    UPROPERTY(meta = (BindWidget))
    UButton* ButtonPlay;

    UPROPERTY(meta = (BindWidget))
    UButton* ButtonExit;

    // 클릭 시 호출될 함수들
    UFUNCTION()
    void OnClickPlay();

    UFUNCTION()
    void OnClickExit();
	
};
