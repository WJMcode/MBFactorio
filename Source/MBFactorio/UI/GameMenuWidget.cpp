// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/GameMenuWidget.h"
#include "Tools/MBFController.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ReplayMenuWidget.h"

void UGameMenuWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ButtonContinue)
    {
        ButtonContinue->OnClicked.AddDynamic(this, &UGameMenuWidget::OnClickContinue);
    }

    if (ButtonReplay)
    {
        ButtonReplay->OnClicked.AddDynamic(this, &UGameMenuWidget::OnClickReplay);
    }

    if (ButtonExit)
    {
        ButtonExit->OnClicked.AddDynamic(this, &UGameMenuWidget::OnClickExit);
    }
}

void UGameMenuWidget::OnClickContinue()
{
    // 게임 시간 다시 흐르게 하기
    UGameplayStatics::SetGamePaused(GetWorld(), false);

    // UI 제거
    this->RemoveFromParent();

    // 마우스 커서 숨기고 입력모드 변경
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        PC->bShowMouseCursor = false;
        FInputModeGameOnly InputMode;
        PC->SetInputMode(InputMode);
    }

    // 커서 UI 재생성
    if (AMBFController* PlayerController = Cast<AMBFController>(UGameplayStatics::GetPlayerController(this, 0)))
    {
        PlayerController->CreateCursorWidget();
    }
}

void UGameMenuWidget::OnClickReplay()
{
    // 현재 GameMenu 위젯 제거
    this->RemoveFromParent();

    // 컨트롤러에 위임
    if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
    {
        if (AMBFController* PlayerController = Cast<AMBFController>(PC))
        {
            PlayerController->OpenReplayMenu(); 
        }
    }       
}

void UGameMenuWidget::OnClickExit()
{
    // UI 제거
    this->RemoveFromParent();

    // 게임 시간 재개
    UGameplayStatics::SetGamePaused(GetWorld(), false);

    // 메인 메뉴 레벨(처음 MBFStartWidget이 나오는 레벨)로 이동
    UGameplayStatics::OpenLevel(this, FName("Factorio"));
}
