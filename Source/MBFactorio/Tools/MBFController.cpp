// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/MBFController.h"
#include "Tools/MBFInstance.h"
#include "Struct/MBFStruct.h"
#include "Math/UnrealMathUtility.h"

AMBFController::AMBFController()
{

	ConstructorHelpers::FObjectFinder<UInputMappingContext> Asset
	{ TEXT("/Script/EnhancedInput.InputMappingContext'/Game/Gamemode/InGame/InGameIMC.InGameIMC'") };
	check(Asset.Object);
	IMC_Default = Asset.Object;

	bOpenInventory = false;
}

void AMBFController::BeginPlay()
{
	Super::BeginPlay();

	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	Subsystem->AddMappingContext(IMC_Default, 0);
	SetInputMode(FInputModeGameAndUI());

	if (!MBFInventoryComponent)
	{
		MBFInventoryComponent = NewObject<UMBFInventoryComponent>(this, UMBFInventoryComponent::StaticClass());
		if (MBFInventoryComponent)
		{
			MBFInventoryComponent->RegisterComponent();
		}
	}
	if (!CraftComponent)
	{
		CraftComponent = NewObject<UCraftComponent>(this, UCraftComponent::StaticClass());
		if (CraftComponent)
		{
			
			CraftComponent->RegisterComponent();
			CraftComponent->SetInventoryComponent(MBFInventoryComponent);
		}
	}

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

void AMBFController::Tick(float DeltaSeconds)
{

}




int32 AMBFController::GetInventoryItemCount(FName ItemID)
{
	return MBFInventoryComponent->GetInventoryItemCount(ItemID);
}

// On/Off Inventory
void AMBFController::InventoryTogle()
{
	MBFInventoryComponent->InventoryTogle();

}
