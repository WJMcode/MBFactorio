#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "GroundTile.generated.h"

/**
 * 땅 타일 클래스입니다.
 * 현재 타입 구분용으로 별도 기능이 없습니다. 
 */
UCLASS()
class MBFACTORIO_API AGroundTile : public ATile
{
	GENERATED_BODY()
	
private:
	virtual void InitializeTile(float InTileSize) override;

protected:
	UPROPERTY(EditAnywhere, Category = "Tile")
	TArray<UMaterialInterface*> GroundMaterials;
};
