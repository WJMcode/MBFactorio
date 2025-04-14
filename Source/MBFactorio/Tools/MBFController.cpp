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
	bEnableClickEvents = true;
	bShowMouseCursor = true;
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
		MBFInventoryComponent->AddItem(FName("5"), 50);
		MBFInventoryComponent->AddItem(FName("4"), 50);
		MBFInventoryComponent->AddItem(FName("3"), 50);
		MBFInventoryComponent->AddItem(FName("8"), 50);
		MBFInventoryComponent->SetSortOption(true);
		MBFInventoryComponent->SortInventory();
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





void AMBFController::InventoryTogle()
{

	AMBFHUD* MBFHUD = Cast<AMBFHUD>(GetHUD());
	if (!MBFHUD) return;

	if (FurnaceUIOwnerActor)
	{
		FurnaceUIOwnerActor->CloseFurnace();
		FurnaceUIOwnerActor = nullptr;
	}
	else if (CraftMachineUIOwnerActor)
	{
		CraftMachineUIOwnerActor->CloseCraftMachine();
		CraftMachineUIOwnerActor = nullptr;
	}
	else if (bOpenInventory)
	{
		MBFHUD->CloseInventory();
		bOpenInventory = false;
	}
	else
	{
		MBFHUD->OpenInventory();
		bOpenInventory = true;
		
	}
}