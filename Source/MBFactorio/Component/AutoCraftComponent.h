// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Component/MBFInventoryComponent.h"
#include "Struct/MBFStruct.h"
#include "AutoCraftComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MBFACTORIO_API UAutoCraftComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAutoCraftComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void SetInventoryComponent(UMBFInventoryComponent* InComponent) { InventoryComponent = InComponent; }

	void SetBuildItem(FItemData* InItemData);

	void SetBuildItemNone() { BuildItemData = nullptr; }





private:
	UMBFInventoryComponent* InventoryComponent;
	FItemData* BuildItemData;
	float BuildMTime = 0.f;
};
