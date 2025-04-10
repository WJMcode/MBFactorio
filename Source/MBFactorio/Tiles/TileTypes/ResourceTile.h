#pragma once

#include "CoreMinimal.h"
#include "Tiles/TileBase/Tile.h"
#include "Tiles/TileBase/TileStructs.h"
#include "ResourceTile.generated.h"

//이슈:
//		OverlapBox의 크기가 너무 작음

/**
 * 리소스 타일 클래스입니다.
 * 
 * 생성자에서 아무것도 하고 있지 않기 때문에 
 * 리소스 타일 관련하여 에디터에서 테스트할 때에는,
 * 블루프린트로 상속하여 사용해야 합니다.
 */
UCLASS()
class MBFACTORIO_API AResourceTile : public ATile
{
	GENERATED_BODY()
	
	friend class ATileGridManager;

public:
	AResourceTile();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	// 현재 리소스 타입 가져오기
	EResourceType GetResourceType() const { return ResourceType; }

private:
	void SetResourceType(EResourceType InType);

private:
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OverlapBox;

	// 리소스 타일의 타입
	UPROPERTY(VisibleAnywhere, Category = "Resource")
	EResourceType ResourceType = EResourceType::Unknown;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsPlayerNear = false;
};
