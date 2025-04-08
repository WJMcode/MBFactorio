// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFGamemode.h"
#include "Tools/MBFController.h"
#include "UI/MBFStartWidget.h"

AMBFGamemode::AMBFGamemode()
{

    //순서변경 HUD먼저 설정해야 PlayerController의 HUD도 설정된 값으로 생성됨
    HUDClass = AMBFHUD::StaticClass();
    PlayerControllerClass = AMBFController::StaticClass();

    // 2025.04.08 시작 화면 로직 'Tools/LYJGameMode'로 이동
}

void AMBFGamemode::BeginPlay()
{
    Super::BeginPlay();

    // 2025.04.08 시작 화면 로직 'Tools/LYJGameMode'로 이동
}
