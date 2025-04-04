// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MBFStartWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMBFStartWidget::NativeConstruct()
{
    Super::NativeConstruct();

    if (ButtonPlay)
    {
        ButtonPlay->OnClicked.AddDynamic(this, &UMBFStartWidget::OnClickPlay);
    }

    if (ButtonExit)
    {
        ButtonExit->OnClicked.AddDynamic(this, &UMBFStartWidget::OnClickExit);
    }
}

void UMBFStartWidget::OnClickPlay()
{
    this->RemoveFromParent();

    UGameplayStatics::OpenLevel(this, FName("Factorio"));
}

void UMBFStartWidget::OnClickExit()
{
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
    }
}
