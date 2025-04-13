// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BluePrint/UserWidget.h"
#include "Components/Image.h"
#include "BuildReSetWidget.generated.h"

/**
 * 
 */

class ACraftMachine;

UCLASS()
class MBFACTORIO_API UBuildReSetWidget : public UUserWidget
{
	GENERATED_BODY()
	

	UImage* ResetImage;


	FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent);


	ACraftMachine* OwnerActor;

public:
	void SetOwnerActor(ACraftMachine* InActor) { OwnerActor = InActor; }


};
