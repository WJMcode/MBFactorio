// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Component/MBFInventoryComponent.h"
#include "Tools/Widget/MBFSlot.h"
#include "Tools/Widget/BuildReSetWidget.h"
#include "Components/ProgressBar.h"
#include "Tools/Widget/MBFSlot.h"
#include "AutoCraftWidget.generated.h"

/**
 * 
 */

class ACraftMachine;
UCLASS()
class MBFACTORIO_API UAutoCraftWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UMBFSlot* CharactorSlot[80];
	UPROPERTY()
	UMBFSlot* RequiredItemSlot[5];
	UPROPERTY()
	UBuildReSetWidget* BuildResetWidget;
	UPROPERTY()
	ACraftMachine* OwnerActor;

	
	void virtual NativeConstruct() override;

public:

	void SetOwnerInventory(UMBFInventoryComponent* InComponent) { OwnerInventory = InComponent; }
	void SetMachineInventory(UMBFInventoryComponent* InComponent) { MachineInventory = InComponent; }
	void SetOwnerActor(ACraftMachine* InActor) { OwnerActor = InActor; }
	ACraftMachine* GetOwnerActor() { return OwnerActor; }
	void SetPercent(float Progresspercent) { ProgressBar->SetPercent(Progresspercent); }
	void CraftMachineChanged();
	void SlotChanged(int32 InSlot) { CharactorSlot[InSlot]->Changed(InSlot); }
	void OnChanged();

private:
	UPROPERTY()
	UProgressBar* ProgressBar;
	UPROPERTY()
	UMBFInventoryComponent* OwnerInventory;
	UPROPERTY()
	UMBFInventoryComponent* MachineInventory;

	
};
 