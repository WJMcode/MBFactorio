#include "Tile.h"

ATile::ATile()
{
    PrimaryActorTick.bCanEverTick = false;

	TileMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TileMesh"));
    RootComponent = TileMesh;
}

void ATile::InitializeTile(float InTileSize)
{
	SetTileScale(InTileSize);
	//SetRandomMaterial(TileMaterials);
}

float ATile::GetMeshBaseSize() const
{
	if (!TileMesh || !TileMesh->GetStaticMesh())
	{
		UE_LOG(LogTemp, Error, TEXT("ATile: Mesh가 지정되지 않았습니다!"));
		return 1.f;
	}

	const FBoxSphereBounds Bounds = TileMesh->GetStaticMesh()->GetBounds();
	return Bounds.BoxExtent.X * 2.f; // 또는 Y축 기준 등 선택 가능
}

void ATile::SetTileScale(float TileSize)
{
	float MeshSize = GetMeshBaseSize();
	float Scale = TileSize / MeshSize;
	SetActorScale3D(FVector(Scale, Scale, 1.f)); // 타일은 평면이기 때문에 Z 스케일은 그대로 두기
}

void ATile::SetRandomMaterial(const TArray<UMaterialInterface*>& Materials)
{
	if (Materials.Num() > 0)
	{
		int32 Index = FMath::RandRange(0, Materials.Num() - 1);
		TileMesh->SetMaterial(0, Materials[Index]);
	}
}
