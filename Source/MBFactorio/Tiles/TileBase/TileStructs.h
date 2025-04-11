#pragma once

#include "CoreMinimal.h"
#include "TileStructs.generated.h"

// 리소스 타일의 타입 정의
UENUM(BlueprintType)
enum class EResourceType : uint8
{
	Copper,	// 구리
	Iron,	// 철	
	Stone,	// 돌
	Unknown UMETA(Hidden)
};

// EResourceType 값을 FString으로 변환하여 반환하는 함수입니다.
static FORCEINLINE FString ResourceTypeToString(EResourceType Type)
{
	switch (Type)
	{
	case EResourceType::Copper: return TEXT("Copper");
	case EResourceType::Iron: return TEXT("Iron");
	//case EResourceType::Coal: return TEXT("석탄");
	case EResourceType::Stone: return TEXT("Stone");
	default: return TEXT("알 수 없음");
	}
}

// 리소스 머티리얼 세트
// 광물 타일은 타입마다 머티리얼을 여러 개 설정합니다.
// ex) Copper 타입 : Copper_머티리얼1, Copper_머티리얼2...
// ex) Iron 타입 : Iron_머티리얼1, Iron_머티리얼2...
USTRUCT(BlueprintType)
struct FResourceMaterialSet
{
	GENERATED_BODY()

	// 리소스 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EResourceType ResourceType;

	// 하나의 타입이 여러 개의 머티리얼을 보유할 수 있습니다.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UMaterialInterface*> Materials;
};