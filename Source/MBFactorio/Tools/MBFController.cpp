// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFController.h"

AMBFController::AMBFController()
{

	ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
	{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Gamemode/InGame/InGameIMC.InGameIMC'") };
	check(Asset.Object);
	IMC_Default = Asset.Object;
	InventoryItems.SetNum(80);
	bOpenInventory = false;

}

void AMBFController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(IMC_Default, 0);
	SetInputMode(FInputModeGameAndUI());
}

void AMBFController::SetupInputComponent() {
	Super::SetupInputComponent();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	ensure(EnhancedInputComponent);

	const TArray<FEnhancedActionKeyMapping>& Mappings = IMC_Default->GetMappings();
	for (auto& It : Mappings)
	{
		if (It.Action->GetFName() == TEXT("PressE"))
		{
			const UInputAction* InputAction = It.Action.Get();
			EnhancedInputComponent->BindAction(InputAction, ETriggerEvent::Started, this, &ThisClass::InventoryTogle);
		}
	}
}

//E키가 눌리면 실행되는 함수
void AMBFController::InventoryTogle()
{

	AMBFHUD* HUD = Cast<AMBFHUD>(GetHUD());
	if (bOpenInventory)
	{

		HUD->CloseInventory();
		bOpenInventory = false;
	}
	else
	{
		HUD->OpenInventory();
		bOpenInventory = true;
	}
}
