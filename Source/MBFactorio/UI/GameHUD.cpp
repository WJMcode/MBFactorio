// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameHUD.h"
#include "Components/TextBlock.h"

void UGameHUD::NativeConstruct()
{
    Super::NativeConstruct();

    // 시작 시 초기 텍스트 설정
    UpdateGoalStatus(0, 0);
}

void UGameHUD::UpdateGoalStatus(int32 DrillCount, int32 FurnaceCount)
{
    if (TextBox)
    {
        FString GoalText = FString::Printf(
            TEXT("1. 구조물을 설치하세요.\n2. 화력 채광 드릴: %d / 10\n3. 돌 용광로: %d / 5"),
            DrillCount,
            FurnaceCount
        );

        TextBox->SetText(FText::FromString(GoalText));
    }
}
