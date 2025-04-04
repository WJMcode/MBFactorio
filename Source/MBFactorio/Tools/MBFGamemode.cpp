// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFGamemode.h"
#include "Tools/MBFController.h"
#include "UI/MBFStartWidget.h"

AMBFGamemode::AMBFGamemode()
{

	PlayerControllerClass = AMBFController::StaticClass();

    ConstructorHelpers::FClassFinder<UUserWidget> WidgetClass(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Play.UI_Play_C'"));
    if (WidgetClass.Succeeded())
    {
        StartWidgetClass = WidgetClass.Class;
    }
}

void AMBFGamemode::BeginPlay()
{
    Super::BeginPlay();

    if (StartWidgetClass != nullptr)
    {
        CurrentWidget = CreateWidget<UMBFStartWidget>(GetWorld(), StartWidgetClass);

        if (CurrentWidget != nullptr)
        {
            CurrentWidget->AddToViewport();
        }
    } 
}
