#include "Tiles/TileTypes/StructuresTile.h"
#include "Components/BoxComponent.h"

AStructuresTile::AStructuresTile()
{
	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);
}

void AStructuresTile::BeginPlay()
{
	Super::BeginPlay();

	// OverlapBox의 크기 설정
	//OverlapBox->SetBoxExtent(FVector((18.f, 21.f, 1.f)));
	
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AStructuresTile::OnBoxBeginOverlap);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AStructuresTile::OnBoxEndOverlap);
}

void AStructuresTile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AStructuresTile::OnBoxBeginOverlap"));
}

void AStructuresTile::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("AStructuresTile::OnBoxEndOverlap"));
}

void AStructuresTile::SetStructuresType(EStructuresType InType)
{
	StructuresType = InType;
}

void AStructuresTile::SetStructuresTileScale(EStructuresType InType, float TileSize)
{
	// 화로는 세로가 두 배로 긴 형태이므로 X : Y = 1 : 2의 비율로 크기를 조정합니다.
	if (EStructuresType::Furnace == InType)
	{
		float MeshSize = GetMeshBaseSize();
		float Scale = TileSize / MeshSize;
		SetActorScale3D(FVector(Scale, Scale * 2.f, 1.f)); // 타일은 평면이기 때문에 Z 스케일은 그대로 두기
	}
}
