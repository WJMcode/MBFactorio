// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Tiles/TileBase/TileStructs.h"
#include "TileDataAsset.generated.h"

class ATile;

// 타일의 기본 정보를 담는 구조체입니다.
USTRUCT(BlueprintType)
struct FTileInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ATile> TileClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float TileSize = 100.f;
};

// 타일 데이터 에셋입니다. 
// (ATileGridManager에서 해당 데이터 에셋을 통해 타일을 생성합니다.)
UCLASS(BlueprintType)
class MBFACTORIO_API UTileDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	// 생성할 타일의 개수 (가로)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	int32 GridWidth = 10;

	// 생성할 타일의 개수 (세로)
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grid")
	int32 GridHeight = 10;

/* 땅 타일 관련 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ground")
	FTileInfo GroundTileInfo;

	// 땅 타일은 타입이 없으므로 여러 개의 머티리얼이 모두 같은 타입입니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ground")
	TArray<UMaterialInterface*> GroundTileMaterials;

/* 광물 타일 관련 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	FTileInfo ResourceTileInfo;

	// 여러 개의 타입과 각각 그에 대응하는 머티리얼 세트를 저장합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Resource")
	TArray<FResourceTypeAndMaterials> ResourceTileTypeAndMaterialSet;

/* 구조물 타일 관련 */
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structures")
	FTileInfo StructuresTileInfo;

	// 구조물 타일은 하나의 머티리얼을 사용합니다.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Structures")
	FStructuresTypeAndMaterial StructuresTypeAndMaterial;
};
