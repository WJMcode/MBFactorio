// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/AutoCraftComponent.h"
#include "Tools/MBFController.h"
#include "Tiles/TileMachine/CraftMachineTile.h"

// Sets default values for this component's properties
UAutoCraftComponent::UAutoCraftComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAutoCraftComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UAutoCraftComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BuildItemData)
	{
		ACraftMachineTile* MachineOwner = Cast<AMBFController>(GetWorld()->GetFirstPlayerController())->GetCraftMachineOwner();
		if (MachineOwner)
		{
			MachineOwner->GetAutoCraftUI()->OnChanged();
			MachineOwner->GetAutoCraftUI()->SetPercent(BuildMTime / BuildItemData->BuildTime);
		}
		for (int i = 0; i < BuildItemData->RequiredItems.Num(); i++)
		{
			if (BuildItemData->RequiredItems[i].RequiredCount > InventoryComponent->GetInventoryItem(i).MCount)
			{
				return;
			}
		}
		BuildMTime += DeltaTime;

		if (BuildItemData->BuildTime <= BuildMTime)
		{
			for (int i = 0; i < BuildItemData->RequiredItems.Num(); i++)
			{
				InventoryComponent->GetInventoryIndex(i)->MCount -= BuildItemData->RequiredItems[i].RequiredCount;
			}

			InventoryComponent->GetInventoryIndex(4)->MCount += BuildItemData->CreateCount;
			
			BuildMTime -= BuildItemData->BuildTime;
		}
		
	}
	// ...
}

void UAutoCraftComponent::SetBuildItem(FItemData* InItemData)
{
	BuildMTime = 0.f;
	BuildItemData = InItemData;
	
}

