// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileDataAsset.h"
#include "TileGridManager.generated.h"

/*
 * 타일을 레벨에 배치하는 매니저 클래스입니다.
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
	// 땅 타일을 생성하는 함수
	void SpawnGroundTiles();
	// 리소스(광물) 타일을 생성하는 함수
	void SpawnResourceTiles();
	// 구조물 타일을 생성하는 함수
	void SpawnStructuresTile();

	/**
	 * @param TileClass			 생성할 타일 명시 (땅, 광물 등)
	 * @param SpawnProbability   타일이 생성될 확률 (0.0 ~ 1.0)
	 * @param ZOffset			 생성 위치의 Z축 보정값(예 : 광물을 땅 위에 살짝 띄우는 용도)
	 * @param InTileSize		 타일의 크기 설정
	 * @param InRotator			 타일에 적용할 회전값
	 * @param bUseRandomRotation 랜덤 회전값 사용 여부
	 */
	void SpawnTiles(TSubclassOf<class ATile> TileClass, float SpawnProbability, float ZOffset, float InTileSize, FRotator InRotator = FRotator::ZeroRotator, bool bUseRandomRotation = true);

private:
	// 각종 타일 정보를 데이터 에셋에 세팅하고, 그 값을 통해 타일을 생성합니다.
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	UTileDataAsset* TileDataAsset;
};
