// Fill out your copyright notice in the Description page of Project Settings.


#include "Tiles/TileMachine/FurnaceTile.h"
#include "Tools/MBFController.h"

AFurnaceTile::AFurnaceTile()
{

	PrimaryActorTick.bCanEverTick = true;

	InventoryComponent = CreateDefaultSubobject<UMBFInventoryComponent>(TEXT("InventoryComponent"));
	InventoryComponent->SetInventoryType(EInventoryType::Furnace);
	BakeComponent = CreateDefaultSubobject<UBakeComponent>(TEXT("BakeComponent"));
	BakeComponent->SetComponentTickEnabled(true);
	BakeComponent->SetAutoActivate(true);

}

void AFurnaceTile::BeginPlay()
{
	Super::BeginPlay();
	OnClicked.AddDynamic(this, &ThisClass::OpenFurnace);
	if (BakeComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("BakeComponent: IsRegistered=%d | IsActive=%d | IsComponentTickEnabled=%d"),
			BakeComponent->IsRegistered(),
			BakeComponent->IsActive(),
			BakeComponent->IsComponentTickEnabled());
	}
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

void AFurnaceTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFurnaceTile::OpenFurnace(AActor* TouchedActor, FKey ButtonPressed)
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
	PC->RemoveCursorWidget(); // 커서 위젯 삭제 
}

void AFurnaceTile::CloseFurnace()
{
	FurnaceUI->RemoveFromViewport();
}
