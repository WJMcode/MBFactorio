// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/BakeComponent.h"
#include "Component/MBFInventoryComponent.h"

// Sets default values for this component's properties
UBakeComponent::UBakeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	MaxEnergy = 4000000.f;
	EnergyEfficiency = 1.f;


	// ...
}


// Called when the game starts
void UBakeComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame




void UBakeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void UBakeComponent::SetInventoryComponent(UMBFInventoryComponent* IVC)
{
	InventoryComponent = IVC;
	if (InventoryComponent)
	{
		InventoryComponent->SetInventoryNum(3);
		InventoryComponent->SetSortOption(false);
		InventoryComponent->SetInventoryType(EInventoryType::Furnace);
	}
}