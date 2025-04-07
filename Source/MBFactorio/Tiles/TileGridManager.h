// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ResourceTile.h"
#include "TileGridManager.generated.h"

// 머티리얼 세트 구조체
USTRUCT(BlueprintType)
struct FResourceMaterialSet
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType ResourceType = EResourceType::Unknown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> Materials;
};

/*
 * ATile에서 지정한 타일을 레벨에 배치하는 매니저 클래스입니다.
 */
UCLASS()
class MBFACTORIO_API ATileGridManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ATileGridManager();

protected:
	virtual void BeginPlay() override;

private:
	/**
	 * @param Category			 생성할 타일의 종류 (땅, 광물 등)
	 * @param TileClass			 생성할 타일 명시 (땅, 광물 등)
	 * @param SpawnProbability   타일이 생성될 확률 (0.0 ~ 1.0)
	 * @param ZOffset			 생성 위치의 Z축 보정값(예 : 광물을 땅 위에 살짝 띄우는 용도)
	 * @param InTileSize		 타일의 크기 설정
	 * @param bUseRandomRotation 랜덤 회전값 사용 여부
	 */
	void SpawnTiles(TSubclassOf<ATile> TileClass, float SpawnProbability, float ZOffset, float InTileSize, FRotator InRotator = FRotator::ZeroRotator, bool bUseRandomRotation = true);

	// 땅 타일을 생성하는 함수
	void SpawnGroundTiles();
	// 광물 등 다른 리소스 타일을 생성하는 함수
	void SpawnResourceTiles();

/* 아래 멤버 변수들은 블루프린트로 ATileGridManager를 상속받아 값을 설정합니다. */
protected:
	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> GroundTileClass;

	UPROPERTY(EditAnywhere, Category = "Tiles")
	TSubclassOf<ATile> ResourceTileClass;

	// 에디터에서 설정: 리소스 타입별 머티리얼 세트
	UPROPERTY(EditAnywhere, Category = "Resource")
	TArray<FResourceMaterialSet> ResourceMaterialSets;
protected:
	UPROPERTY(EditAnywhere)
	int32 GridWidth = 10;

	UPROPERTY(EditAnywhere)
	int32 GridHeight = 10;

	UPROPERTY(EditAnywhere)
	float GroundTileSize = 100.f;

	UPROPERTY(EditAnywhere)
	float ResourceTileSize = 25.f;
};
