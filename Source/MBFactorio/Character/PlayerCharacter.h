#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "PlayerCharacter.generated.h"

/*
 * 플레이어가 조종하는 캐릭터입니다. 
 */
UCLASS()
class MBFACTORIO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 플레이어 입력을 처리하기 위해 호출되는 함수
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	// 캐릭터 이동을 위한 함수
	void MoveCharacter(const FInputActionValue& Value);
	
/* 플레이어 움직임 입력 관련 */
protected:
	/* 아래 멤버 변수들은 에디터에서 세팅합니다. */
	// IMC
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* MoveMappingContext;
	
	// IA
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;
};
