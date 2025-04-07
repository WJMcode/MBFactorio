// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGridManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

ATileGridManager::ATileGridManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ATileGridManager::BeginPlay()
{
	Super::BeginPlay();

    SpawnGroundTiles();
    SpawnResourceTiles();
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
			FRotator Rotation = bUseRandomRotation
				? FRotator(0.f, FMath::RandRange(0, 3) * 90, 0.f) // 타일을 0, 90, 180, 270도 중 하나로 회전시킴
				: InRotator;

			ATile* NewTile = GetWorld()->SpawnActor<ATile>(TileClass, Location, Rotation);
			if (NewTile)
			{
				// 만약 ResourceTile이면 타입/머티리얼 세팅
				if (AResourceTile* ResourceTile = Cast<AResourceTile>(NewTile))
				{
					if (ResourceMaterialSets.Num() > 0)
					{
						int32 Index = FMath::RandRange(0, ResourceMaterialSets.Num() - 1);
						const FResourceMaterialSet& SelectedSet = ResourceMaterialSets[Index];

						ResourceTile->SetRandomMaterial(SelectedSet.Materials);
						ResourceTile->SetResourceType(SelectedSet.ResourceType);
					}
				}

				// 타일 생성 후 각종 초기화 작업 진행
				NewTile->InitializeTile(InTileSize);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("타일 스폰 실패: (%d, %d)"), X, Y);
			}
		}
	}
}

void ATileGridManager::SpawnGroundTiles()
{
	SpawnTiles(GroundTileClass, 1.0f, 0.f, GroundTileSize, FRotator::ZeroRotator, true);
}

void ATileGridManager::SpawnResourceTiles()
{
	SpawnTiles(ResourceTileClass, 0.3f, 0.1f, ResourceTileSize, FRotator(0.f, 90.f, 0.f), false);
}