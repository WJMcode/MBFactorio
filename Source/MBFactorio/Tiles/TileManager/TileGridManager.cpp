#include "TileGridManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Tiles/TileBase/TileStructs.h"
#include "Tiles/TileTypes/GroundTile.h"
#include "Tiles/TileTypes/ResourceTile.h"
#include "Tiles/TileTypes/StructuresTile.h"

ATileGridManager::ATileGridManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATileGridManager::BeginPlay()
{
	Super::BeginPlay();

    SpawnGroundTiles();
    SpawnResourceTiles();
    SpawnStructuresTile();
}

void ATileGridManager::SpawnGroundTiles()
{
	const FTileInfo& GroundTileInfo = TileDataAsset->GroundTileInfo;
	SpawnTiles(GroundTileInfo.TileClass, 1.0f, 0.f, GroundTileInfo.TileSize, FRotator::ZeroRotator, true);
}

void ATileGridManager::SpawnResourceTiles()
{
	const FTileInfo& ResourceTileInfo = TileDataAsset->ResourceTileInfo;
	SpawnTiles(ResourceTileInfo.TileClass, 0.3f, 0.1f, ResourceTileInfo.TileSize, FRotator(0.f, 90.f, 0.f), false);
}

void ATileGridManager::SpawnStructuresTile()
{
	const FTileInfo& StructuresTileInfo = TileDataAsset->StructuresTileInfo;
	SpawnTiles(StructuresTileInfo.TileClass, 1.0f, 0.1f, StructuresTileInfo.TileSize, FRotator(0.f, 90.f, 0.f), false);
}

void ATileGridManager::SpawnTiles(TSubclassOf<ATile> TileClass, float SpawnProbability, float ZOffset, float InTileSize, FRotator InRotator, bool bUseRandomRotation)
{
	// 캐릭터 가져오기
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (!PlayerCharacter || !TileClass) 
	{
		UE_LOG(LogTemp, Warning, TEXT("타일 생성 실패: 캐릭터 또는 TileClass 없음"));
		return;
	}

	// 캐릭터 발바닥 높이 계산
	float HalfHeight = PlayerCharacter->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	FVector PlayerLocation = PlayerCharacter->GetActorLocation();
	float FootZValue = PlayerLocation.Z - HalfHeight;

	// 생성할 타일의 개수
	int32 GridWidth = TileDataAsset->GridWidth;
	int32 GridHeight = TileDataAsset->GridHeight;

	// 구조물 타일인 경우 하나만 생성
	if(TileClass->IsChildOf(AStructuresTile::StaticClass()))
	{
		GridWidth = 1;
		GridHeight = 1;
	}

	// 타일 그리드 중심이 캐릭터 위치로 가도록 보정
	FVector Origin = PlayerLocation - FVector((GridWidth - 1) * InTileSize * 0.5f, (GridHeight - 1) * InTileSize * 0.5f, 0.f);

	for (int32 Y = 0; Y < GridHeight; ++Y)
	{
		for (int32 X = 0; X < GridWidth; ++X)
		{
			
			// 0.0 ~ 1.0 사이의 난수를 뽑아서 확률적으로 타일을 생성
			// Resource 타일은 부분적으로 생성하기 위함.
			if (FMath::FRand() > SpawnProbability) continue;

			FVector Location = Origin + FVector(X * InTileSize, Y * InTileSize, 0.f);
			Location.Z = FootZValue + ZOffset;	// 발바닥 높이에 생성

			// bUseRandomRotation가 ture면 랜덤 회전값 적용
			// false면 인자로 받은 회전값 사용
			FRotator Rotation = 
										// 타일을 0, 90, 180, 270도 중 하나로 회전
				bUseRandomRotation ? FRotator(0.f, FMath::RandRange(0, 3) * 90, 0.f) : InRotator;

			ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, Location, Rotation);
			if (NewTile)
			{
				// 타일 크기 설정
				NewTile->SetTileScale(InTileSize);

				// 땅 타일일 경우 머티리얼만 랜덤으로 설정
				if (AGroundTile* GroundTile = Cast<AGroundTile>(NewTile))
				{
					if (TileDataAsset && TileDataAsset->GroundTileMaterials.Num() > 0)
					{
						GroundTile->SetRandomTileMaterial(TileDataAsset->GroundTileMaterials);
					}
				}
				// 리소스 타일일 경우 타입 설정 및 랜덤 머티리얼 설정
				else if (AResourceTile* ResourceTile = Cast<AResourceTile>(NewTile))
				{
					if (TileDataAsset && TileDataAsset->ResourceTileTypeAndMaterialSet.Num() > 0)
					{
						const TArray<FResourceTypeAndMaterials>& Sets = TileDataAsset->ResourceTileTypeAndMaterialSet;
						int32 Index = FMath::RandRange(0, Sets.Num() - 1);
						// Sets에 있는 여러 개의 타입 중 하나를 뽑아, 
						// 타입과 머티리얼 정보를 SelectedSet에 저장
						const FResourceTypeAndMaterials& SelectedSet = Sets[Index];

						// SelectedSet에 담긴 타입 정보로 생성된 타일 타입을 설정
						ResourceTile->SetResourceType(SelectedSet.ResourceType);
						// SelectedSet에 담긴 머티리얼 배열을 통해 생성된 타일의 머티리얼을 설정
						ResourceTile->SetRandomTileMaterial(SelectedSet.Materials);
					}
				}
				// 구조물 타일일 경우 한 가지의 머티리얼로 세팅
				else if (AStructuresTile* StructuresTile = Cast<AStructuresTile>(NewTile))
				{
					// StructuresTypeAndMaterial에 담긴 타입 정보로 생성된 타일 타입을 설정
					StructuresTile->SetStructuresType(TileDataAsset->StructuresTypeAndMaterial.StructuresType);
					// StructuresTypeAndMaterial에 담긴 머티리얼을 통해 생성된 타일의 머티리얼을 설정
					StructuresTile->SetTileMaterial(TileDataAsset->StructuresTypeAndMaterial.Material);
					// 구조물 타입에 따라 크기를 조정 (예: 화로는 세로로 2칸 차지)
					StructuresTile->SetStructuresTileScale(StructuresTile->GetStructuresType(), InTileSize);
				}
				else
				{
					UE_LOG(LogTemp, Warning, TEXT("잘못된 타일 클래스입니다."));
				}
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("타일 스폰 실패: (%d, %d)"), X, Y);
			}
		}
	}
}
