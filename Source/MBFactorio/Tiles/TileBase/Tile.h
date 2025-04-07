// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

/*
 * 레벨에 생성할 타일(땅, 광물)을 지정하는 클래스입니다.
 */
UCLASS()
class MBFACTORIO_API ATile : public AActor
{
	GENERATED_BODY()
	
	friend class ATileGridManager;

public:	
	ATile();

protected:
	// Mesh의 사이즈를 얻어오는 함수입니다.
	float GetMeshBaseSize() const;

	// 타일의 크기를 설정합니다.
	void SetTileScale(float TileSize);
		
	// 배열에 저장된 머티리얼 하나를 뽑아 머티리얼로 설정합니다.
	void SetRandomMaterial(const TArray<UMaterialInterface*>& Materials);

/* 블루프린트로 ATile를 상속받아 아래 멤버 변수들을 지정합니다. */
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* TileMesh;
};
