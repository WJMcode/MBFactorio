#pragma once

#include "CoreMinimal.h"
#include "Tiles/TileBase/Tile.h"
#include "Tiles/TileBase/TileStructs.h"
#include "StructuresTile.generated.h"

/**
 * 구조물 타일입니다. 현재 화로 기능을 하는 타일입니다.
 */
UCLASS()
class MBFACTORIO_API AStructuresTile : public ATile
{
	GENERATED_BODY()
	
public:
	AStructuresTile();

protected:
	virtual void BeginPlay() override;

	// 캐릭터와 오버랩 발생
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 캐릭터와 오버랩 해제
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// 현재 구조물 타입 가져오기
	EStructuresType GetStructuresType() const { return StructuresType; }

	// 구조물 타입을 설정합니다.
	void SetStructuresType(EStructuresType InType);

	// 구조물 타입에 따라 크기를 다르게 설정합니다.
	void SetStructuresTileScale(EStructuresType InType, float TileSize);

private:
	// 캐릭터와 구조물 사이의 오버랩 판정을 위한 BoxComponent입니다.
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OverlapBox;

	// 구조물 타일의 타입
	UPROPERTY(VisibleAnywhere, Category = "Structures")
	EStructuresType StructuresType = EStructuresType::Unknown;
};
