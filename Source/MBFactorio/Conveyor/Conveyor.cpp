// Fill out your copyright notice in the Description page of Project Settings.


#include "Conveyor/Conveyor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"

AConveyor::AConveyor()
{
	PrimaryActorTick.bCanEverTick = true;

	// 루트 컴포넌트
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// Box
	ConveyorBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	ConveyorBox->SetupAttachment(RootComponent);

	// StaticMesh
	ConveyorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	ConveyorMesh->SetupAttachment(RootComponent);
}

void AConveyor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ConveyorBox) return;

	// 박스 컴포넌트에서 겹치는 액터들을 가져옴
	TArray<AActor*> OverlappingActors;
	ConveyorBox->GetOverlappingActors(OverlappingActors);

	// 액터(컨베이어)의 Forward 방향으로 이동
	FVector Direction = GetActorForwardVector();
	FVector Movement = Direction * Speed * DeltaTime;

	for (AActor* Actor : OverlappingActors)
	{
		if (Actor && Actor != this)
		{
			Actor->AddActorWorldOffset(Movement, true); // Sweep 체크
		}
	}
}