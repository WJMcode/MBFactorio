#pragma once

#include "CoreMinimal.h"
#include "Tiles/TileBase/Tile.h"
#include "Tiles/TileBase/TileStructs.h"
#include "ResourceTile.generated.h"

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
	
public:
	AResourceTile();

protected:
	virtual void BeginPlay() override;

	// 오버랩 발생
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	// 오버랩 해제
	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// 현재 리소스 타입 가져오기
	EResourceType GetResourceType() const { return ResourceType; }

public:
	// 리소스 타입을 설정합니다.
	void SetResourceType(EResourceType InType);

private:
	// 캐릭터와 광물 사이의 오버랩 판정을 위한 BoxComponent입니다.
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* OverlapBox;

	// 리소스 타일의 타입
	UPROPERTY(VisibleAnywhere, Category = "Resource")
	EResourceType ResourceType = EResourceType::Unknown;

protected:
	// 자신(광물) 입장에서 플레이어가 근처에 있는지에 대한 플래그입니다.
	UPROPERTY(BlueprintReadOnly, Category = "State")
	bool bIsPlayerNear = false;
};
