// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/LYJGameMode.h"
#include "Tools/LYJController.h"
#include "UI/MBFStartWidget.h"

ALYJGameMode::ALYJGameMode()
{
    PlayerControllerClass = ALYJController::StaticClass();

    // 블루프린트 플레이어 클래스 지정
    ConstructorHelpers::FClassFinder<APawn> PlayerClass(TEXT("/Game/LYJ_Test/BP_TestPlayer.BP_TestPlayer_C"));
    if (PlayerClass.Succeeded())
    {
        DefaultPawnClass = PlayerClass.Class;
    }

    // 시작 위젯 지정
    ConstructorHelpers::FClassFinder<UMBFStartWidget> WidgetClass(TEXT("/Game/UI/UI_Play.UI_Play_C"));
    if (WidgetClass.Succeeded())
    {
        StartWidgetClass = WidgetClass.Class;
    }
}

void ALYJGameMode::BeginPlay()
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