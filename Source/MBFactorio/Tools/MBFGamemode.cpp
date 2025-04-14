// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFGamemode.h"
#include "Tools/MBFController.h"
#include "UI/MBFStartWidget.h"

AMBFGamemode::AMBFGamemode()
{
    //순서변경 HUD먼저 설정해야 PlayerController의 HUD도 설정된 값으로 생성됨
    HUDClass = AMBFHUD::StaticClass();
    PlayerControllerClass = AMBFController::StaticClass();
    /*ConstructorHelpers::FClassFinder<UMBFStartWidget> WidgetClass(TEXT("/Game/UI/UI_Play.UI_Play_C"));
    if (WidgetClass.Succeeded())
    {
        StartWidgetClass = WidgetClass.Class;
    }*/ // 테스트 시 호옥시 몰라서 삭제 말고 주석처리
}

void AMBFGamemode::BeginPlay()
{
    Super::BeginPlay();

    FString CurrentLevelName = GetWorld()->GetMapName();
    CurrentLevelName.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    if (CurrentLevelName == TEXT("Factorio"))
    {
        if (StartWidgetClass)
        {
            StartWidget = CreateWidget<UMBFStartWidget>(GetWorld(), StartWidgetClass);
            if (StartWidget)
            {
                StartWidget->AddToViewport(); // 시작화면 필요 X 시 주석 걸 것
            }
        }
    }
}
