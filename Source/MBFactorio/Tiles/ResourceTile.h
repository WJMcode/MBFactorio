#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "ResourceTile.generated.h"

// 리소스 타입 정의 (필요에 따라 확장 가능)
UENUM(BlueprintType)
enum class EResourceType : uint8
{
    Copper,
    Iron,
    Stone,
    Unknown UMETA(Hidden)
};

/**
 * 리소스 타일 클래스입니다.
 * 현재 타입 구분용으로 별도 기능이 없습니다.
 */
UCLASS()
class MBFACTORIO_API AResourceTile : public ATile
{
	GENERATED_BODY()
	
private:
	// 생성자
	AResourceTile();

	// 타일 초기화 (타입 랜덤 설정 포함)
	//virtual void InitializeTile(float InTileSize) override;
public:
	void SetResourceType(EResourceType InType);
	// 현재 리소스 타입 가져오기
	EResourceType GetResourceType() const { return ResourceType; }

	// 선택된 리소스 타입
	UPROPERTY(VisibleAnywhere, Category = "Resource")
	EResourceType ResourceType = EResourceType::Unknown;
};
