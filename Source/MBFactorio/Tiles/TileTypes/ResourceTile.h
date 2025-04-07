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
	
	friend class ATileGridManager;

public:
	AResourceTile();
	// 현재 리소스 타입 가져오기
	EResourceType GetResourceType() const { return ResourceType; }

private:
	void SetResourceType(EResourceType InType);

private:
	// 리소스 타일의 타입
	UPROPERTY(VisibleAnywhere, Category = "Resource")
	EResourceType ResourceType = EResourceType::Unknown;
};
