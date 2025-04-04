// Fill out your copyright notice in the Description page of Project Settings.


#include "Conveyor/Conveyor.h"
#include "Components/BoxComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"

// Sets default values
AConveyor::AConveyor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);
    Box->SetRelativeScale3D(FVector(8.f, 2.5f, 0.2f));

    Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
    Arrow->SetupAttachment(RootComponent);
    Arrow->SetRelativeLocation(FVector(-180.f, -90.f, 0.f));
    Arrow->SetRelativeRotation(FRotator(0.f, 180.f, 0.f));

    ConveyorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    ConveyorMesh->SetupAttachment(RootComponent);
    ConveyorMesh->SetRelativeScale3D(FVector(5.f, 1.5f, 0.1f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> ConveyorAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> ConveyorMatAsset(TEXT("/Script/Engine.Material'/Game/ConvayorBelt/M_Conveyor.M_Conveyor'"));
    if (ConveyorAsset.Succeeded())
    {
        ConveyorMesh->SetStaticMesh(ConveyorAsset.Object);
        ConveyorMesh->SetMaterial(0, ConveyorMatAsset.Object);
    }
}

// Called when the game starts or when spawned
void AConveyor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AConveyor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    TArray<AActor*> OverlappingActors;
    Box->GetOverlappingActors(OverlappingActors);

    FVector Direction = Arrow->GetForwardVector();
    FVector Movement = Direction * Speed * DeltaTime;

    for (AActor* Actor : OverlappingActors)
    {
        if (Actor && Actor != this)
        {
            Actor->AddActorWorldOffset(Movement, true); // Sweep = true
        }
    }
}

