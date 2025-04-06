// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFGamemode.h"
#include "Tools/MBFController.h"
#include "UI/MBFStartWidget.h"

AMBFGamemode::AMBFGamemode()
{

	PlayerControllerClass = AMBFController::StaticClass();
    HUDClass = AMBFHUD::StaticClass();
    ConstructorHelpers::FClassFinder<UMBFStartWidget> WidgetClass(TEXT("/Game/UI/UI_Play.UI_Play_C"));
    if (WidgetClass.Succeeded())
    {
        StartWidgetClass = WidgetClass.Class;
    }
}

void AMBFGamemode::BeginPlay()
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
                //CurrentWidget->AddToViewport(); // 시작 화면까지 PIE 로 재생할 시 주석 풀기
            }
        }
    }
}
