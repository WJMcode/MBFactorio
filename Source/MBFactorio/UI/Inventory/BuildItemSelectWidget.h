// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"


#include "Tools/Widget/CraftMachineSlot.h"


#include "BuildItemSelectWidget.generated.h"

/**
 * 
 */
class ACraftMachine;

UCLASS()
class MBFACTORIO_API UBuildItemSelectWidget : public UUserWidget
{
	GENERATED_BODY()
	
	UPROPERTY()
	UCraftMachineSlot* CraftMachineSlot[70];

	virtual void NativeConstruct() override;


	
public:
	
	void OnChanged();
	void SetOwnerActor(ACraftMachine* InActor) { OwnerActor = InActor; }
	


private:
	UPROPERTY()
	ACraftMachine* OwnerActor;
};
