// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Conveyor.generated.h"

UCLASS()
class MBFACTORIO_API AConveyor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AConveyor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
    // 컴포넌트
    UPROPERTY(VisibleAnywhere)
    class UBoxComponent* Box;

    UPROPERTY(VisibleAnywhere)
    class UArrowComponent* Arrow;

    UPROPERTY(VisibleAnywhere)
    class UStaticMeshComponent* ConveyorMesh;

    // 이동 속도
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conveyor")
    float Speed = 200.0f;

    UPROPERTY(VisibleAnywhere)
    bool bGenerateOverlapEvents = false;
};
