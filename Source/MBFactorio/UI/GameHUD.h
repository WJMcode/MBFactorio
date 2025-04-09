// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameHUD.generated.h"

// @TODO 좌측 상단: 목표(타이틀 + 내용(수정 가능하게))
// @TODO 우측 상단: 오브젝트 정보(이미지 + 오브젝트명 + 정보)
// @TODO 중앙 하단: 퀵 인벤토리(드래그앤드롭 가능하게)

UCLASS()
class MBFACTORIO_API UGameHUD : public UUserWidget
{
	GENERATED_BODY()

public:
    // 진행 상황 텍스트를 갱신하는 함수
    UFUNCTION(BlueprintCallable)
    void UpdateGoalStatus(int32 DrillCount, int32 FurnaceCount);

protected:
    virtual void NativeConstruct() override;

    // 목표 텍스트 박스에 연결된 위젯 (UMG에서 바인딩 필요)
    UPROPERTY(meta = (BindWidget))
    class UTextBlock* TextBox;	
};
