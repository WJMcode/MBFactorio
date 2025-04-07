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
    ConstructorHelpers::FClassFinder<UMBFStartWidget> WidgetClass(TEXT("/Game/UI/UI_MBFCursor.UI_MBFCursor_C"));
    if (WidgetClass.Succeeded())
    {
        StartWidgetClass = WidgetClass.Class;
    }
}

void ALYJGameMode::BeginPlay()
{
    Super::BeginPlay();

    FString CurrentMap = GetWorld()->GetMapName();
    CurrentMap.RemoveFromStart(GetWorld()->StreamingLevelsPrefix);

    if (CurrentMap.Equals("Factorio"))
    {
        if (StartWidgetClass != nullptr)
        {
            CurrentWidget = CreateWidget<UMBFStartWidget>(GetWorld(), StartWidgetClass);
            if (CurrentWidget)
            {
                CurrentWidget->AddToViewport(); // 시작 화면까지 PIE 로 재생할 시 주석 풀기
            }
        }
    }
}
