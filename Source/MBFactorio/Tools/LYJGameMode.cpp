// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/LYJGameMode.h"
#include "Tools/LYJController.h"
#include "UI/MBFStartWidget.h"
 
ALYJGameMode::ALYJGameMode()
{
    PlayerControllerClass = ALYJController::StaticClass();

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