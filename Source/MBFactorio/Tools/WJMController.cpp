// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/WJMController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Character/PlayerCharacter.h"

void AWJMController::SetupInputComponent()
{
    Super::SetupInputComponent();

    if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
    {
        // 인벤토리 관련 액션
        EnhancedInput->BindAction(ToggleInventoryAction, ETriggerEvent::Started, this, &AWJMController::ToggleInventory);
        // ESC로 닫기
        EnhancedInput->BindAction(CancelAction, ETriggerEvent::Started, this, &AWJMController::CloseInventory);
    }
}

void AWJMController::BeginPlay()
{
    Super::BeginPlay();

    bShowMouseCursor = true;

    if (APlayerController* PC = Cast<APlayerController>(this))
    {
        if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
            ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
            {
                // IMC 등록
                Subsystem->AddMappingContext(InventoryMappingContext, 0);
            }
    }
}

void AWJMController::ToggleInventory()
{
    if (InventoryWidget)
    {
        CloseInventory();
    }
    else
    {
        OpenInventory();
    }
}

void AWJMController::OpenInventory()
{
    if (!InventoryWidget && InventoryWidgetClass)
    {
        InventoryWidget = CreateWidget<UUserWidget>(this, InventoryWidgetClass);
        InventoryWidget->AddToViewport();

        // 마우스로 UI 클릭도 하고, 게임도 조작 가능
        SetInputMode(FInputModeGameAndUI());
    }
}

void AWJMController::CloseInventory()
{
    if (InventoryWidget)
    {
        InventoryWidget->RemoveFromParent();  // 화면에서 Widget 제거
        InventoryWidget = nullptr;            // 포인터 초기화    
    }
}
