// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/WJMTestHUD/WJMTestHUD.h"
#include "Components/ProgressBar.h"
#include "Components/VerticalBox.h"
#include "UI/Mining/MiningInteractionWidget.h"
#include "Character/PlayerCharacter.h"
#include "Component/Mining/MiningComponent.h"


void AWJMTestHUD::BeginPlay()
{
    Super::BeginPlay();

    if (MiningInteractionWidgetClass)
    {
        MiningInteractionWidget = CreateWidget<UMiningInteractionWidget>(GetWorld(), MiningInteractionWidgetClass);
        if (MiningInteractionWidget)
        {
            MiningInteractionWidget->AddToViewport();
            // 처음에는 Widget을 숨김
            MiningInteractionWidget->SetVisibility(ESlateVisibility::Hidden);
        }
    }

    if (APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwningPlayerController()->GetPawn()))
    {
        if (UMiningComponent* MiningComponent = PlayerCharacter->FindComponentByClass<UMiningComponent>())
        {
            // 채굴 컴포넌트에서 Broadcast하는 이벤트에 바인딩합니다.
            MiningComponent->OnMiningProgress.AddDynamic(this, &AWJMTestHUD::UpdateMiningProgress);
            MiningComponent->OnMiningComplete.AddDynamic(this, &AWJMTestHUD::ShowMiningComplete);
            MiningComponent->OnMiningStopped.AddDynamic(this, &AWJMTestHUD::HideMiningProgress);
        }
    }
}

void AWJMTestHUD::UpdateMiningProgress(float Progress)
{
	if (MiningInteractionWidget)
	{
		MiningInteractionWidget->SetVisibility(ESlateVisibility::Visible);
		MiningInteractionWidget->MiningProgressBar->SetVisibility(ESlateVisibility::Visible);
		//MiningInteractionWidget->MiningCompleteTextBox->SetVisibility(ESlateVisibility::Hidden);
		// 실제로 ProgressBar를 업데이트하는 부분
        MiningInteractionWidget->SetMiningProgress(Progress);
	}
}

void AWJMTestHUD::ShowMiningComplete(EResourceType Resource)
{
	if (MiningInteractionWidget)
	{
		//MiningInteractionWidget->MiningCompleteTextBox->SetVisibility(ESlateVisibility::Visible);
        // 실제로 CompleteText를 출력하는 부분
        MiningInteractionWidget->ShowMiningCompleteMessage(Resource);
	}
}

void AWJMTestHUD::HideMiningProgress()
{
    if (MiningInteractionWidget)
    {
        MiningInteractionWidget->MiningProgressBar->SetVisibility(ESlateVisibility::Hidden);
    }
}
