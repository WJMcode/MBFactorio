// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Component/CraftComponent.h"
#include "Struct/MBFStruct.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Tools/MBFHUD.h"
#include "Misc/Misc.h"
#include "Component/MBFInventoryComponent.h"
#include "Test/TestActor.h"
#include "Test/CraftMachine.h"
#include "MBFController.generated.h"

/**
 *
 */
UCLASS()
class MBFACTORIO_API AMBFController : public APlayerController
{
	GENERATED_BODY()


	UInputMappingContext* IMC_Default;


	AMBFController();
	
public:
	UMBFInventoryComponent* GetInventoryComponent() { return MBFInventoryComponent; }
	UCraftComponent* GetCraftComponent() { return CraftComponent; }


	void SetFurnaceOwner(ATestActor* InActor) { FurnaceUIOwnerActor = InActor; }
	ATestActor* GetFurnaceOwner() { return FurnaceUIOwnerActor; }



	void SetCraftMachineOwner(ACraftMachine* InActor) { CraftMachineUIOwnerActor = InActor; }
	ACraftMachine* GetCraftMachineOwner() { return CraftMachineUIOwnerActor; }


	bool bOpenInventory;



protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

private:

	
	void InventoryTogle();


private:
	UMBFInventoryComponent* MBFInventoryComponent;
	UCraftComponent* CraftComponent;
	ATestActor* FurnaceUIOwnerActor;
	ACraftMachine* CraftMachineUIOwnerActor;


};
