// Fill out your copyright notice in the Description page of Project Settings.


#include "Test/TestActor.h"
#include "Tools/MBFHUD.h"
#include "Tools/MBFController.h"

// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InventoryComponent = CreateDefaultSubobject<UMBFInventoryComponent>(TEXT("InventoryComponent"));
	BakeComponent = CreateDefaultSubobject<UBakeComponent>(TEXT("BakeComponent"));

	
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
    Super::BeginPlay();

    // Setup Box
    BoxComponent = NewObject<UBoxComponent>(this, TEXT("ClickableBox"));
    BoxComponent->RegisterComponent();
    RootComponent = BoxComponent;

    BoxComponent->SetBoxExtent(FVector(50.f));
    BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComponent->SetCollisionResponseToAllChannels(ECR_Ignore);
    BoxComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

    OnClicked.AddDynamic(this, &ATestActor::OpenFurnace);

    InventoryComponent->SetInventoryNum(3);
    BakeComponent->SetInventoryComponent(InventoryComponent);

	UWorld* World = GetWorld();

	UClass* WidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Gamemode/InGame/FurnaceInventory.FurnaceInventory_C'"));
	if (!WidgetClass)
	{
		UE_LOG(LogTemp, Error, TEXT("FurnaceInventory 위젯 클래스 로딩 실패"));
	}

	FurnaceUI = CreateWidget<UFurnaceInventory>(World, WidgetClass);
	if (!FurnaceUI)
	{
		UE_LOG(LogTemp, Error, TEXT("FurnaceUI 생성 실패"));
	}
	FurnaceUI->SetOwnerInventory(InventoryComponent);

}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATestActor::OpenFurnace(AActor* TouchedActor, FKey ButtonPressed)

{
	AMBFController* PC = Cast<AMBFController>(GetWorld()->GetFirstPlayerController());

	if (PC->GetFurnaceOwner())
	{
		return;
	}
	else if (PC->GetCraftMachineOwner())
	{
		return;
	}
	else if (PC->bOpenInventory)
	{
		return;
	}

	FurnaceUI->AddToViewport();
	
	PC->SetFurnaceOwner(this);
}

void ATestActor::CloseFurnace()
{
	FurnaceUI->RemoveFromViewport();
}

