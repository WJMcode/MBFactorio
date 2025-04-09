// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ReplayMenuWidget.h"
#include "Tools/LYJController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UReplayMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ButtonYes)
    {
        ButtonYes->OnClicked.AddDynamic(this, &UReplayMenuWidget::OnClickYes);
    }

    if (ButtonNo)
    {
        ButtonNo->OnClicked.AddDynamic(this, &UReplayMenuWidget::OnClickNo);
    }
}

void UReplayMenuWidget::OnClickYes()
{
    // UI 제거
    this->RemoveFromParent();

    // 게임 시간 재개 (안 해주면 다음 레벨도 정지된 상태일 수 있음)
    UGameplayStatics::SetGamePaused(GetWorld(), false);

    // 현재 레벨 다시 불러오기
    FName CurrentLevel = *UGameplayStatics::GetCurrentLevelName(GetWorld());
    UGameplayStatics::OpenLevel(this, CurrentLevel);
}

void UReplayMenuWidget::OnClickNo()
{
    // UI 제거
    this->RemoveFromParent();

    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (ALYJController* LYJController = Cast<ALYJController>(PC))
        {
            LYJController->bIsGameMenuOpen = false;
            LYJController->ToggleGameMenu();
        }
    }
}
