#pragma once

#include "CoreMinimal.h"
#include "Tiles/TileBase/Tile.h"
#include "GroundTile.generated.h"

/**
 * 땅 타일 클래스입니다.
 * 
 * 현재 타입 구분용으로 별도 기능이 없습니다. 
 * 따라서 땅 타일 관련하여 에디터에서 테스트할 때에는,
 * 블루프린트로 상속하여 사용해야 합니다.
 */
UCLASS()
class MBFACTORIO_API AGroundTile : public ATile
{
	GENERATED_BODY()

public:
};
