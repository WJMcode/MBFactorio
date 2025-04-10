#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Conveyor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class MBFACTORIO_API AConveyor : public AActor
{
	GENERATED_BODY()

public:
	AConveyor();

	virtual void Tick(float DeltaTime) override;

protected:
	// 블루프린트에서 구성한 컴포넌트들을 바인딩
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conveyor")
	UBoxComponent* ConveyorBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conveyor")
	UStaticMeshComponent* ConveyorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conveyor")
	float Speed = 200.0f;

	UPROPERTY(VisibleAnywhere)
	bool bGenerateOverlapEvents = false;
};
