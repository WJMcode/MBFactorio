#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "PlayerCharacter.generated.h"

//@TODO: 
//		 채굴 완료 시 아이템 습득 텍스트 출력
//		 채굴한 광물 인벤토리에 추가
//		 채굴 기능 컴포넌트로 분리 + testHUD 만들어서 HUD에서 위젯 처리하기

// 이슈:
//		채굴 모션 자체가 움직이는 모션이라 루트를 잠금 -> 방향에 따라 모션이 달라지는 문제 발생
//		게임 시작하자마자 밟은 광물은 인식이 안됨

class UInputComponent;
class UMiningComponent;

/*
 * 플레이어가 조종하는 캐릭터입니다. 
 * APlayerCharacter에서 채굴 관련 동작을 시작합니다.
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
	// 플레이어 입력을 처리하기 위해 호출되는 함수
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

public:
	UMiningComponent* GetMiningComponent() const { return MiningComponent; }

protected:
	// 캐릭터 이동을 위한 함수
	void MoveCharacter(const FInputActionValue& Value);
	
/* 채굴 관련 함수 */
public:
	// 곡괭이 메시를 화면에 보이거나 숨깁니다.
	void ShowPickaxe(bool bVisible);

private:
	// 우클릭을 유지하여 채굴 시도
	void OnMiningTriggered();
	// 우클릭 떼어 채굴을 멈춥니다.
	void OnMiningReleased();

public:
	// 채굴 몽타주 재생
	void PlayMiningAnimation();
	// 채굴 몽타주를 멈춥니다.
	void StopMiningAnimation();	
	
private:
	// 채굴 몽타주가 다 재생되면 자동으로 호출되는 함수입니다.
	UFUNCTION()
	void OnMiningMontageEnded(UAnimMontage* Montage, bool bInterrupted);

/* 아이템 드랍 관련 함수 */
private:
	// Z를 유지하여 연속으로 아이템 드랍 
	void OnDropItemTriggered();
	// Z를 눌러 아이템 드랍
	void OnDropItemStarted();
	// Z를 떼면 아이템 드랍을 멈춤
	void OnDropItemReleased();

/* 플레이어 움직임 관련 변수 */
protected:
	// IMC
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* MoveMappingContext;
	
	// IA
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;

/* 채굴 관련 변수 */
protected:
	// IMC
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* WorkMappingContext;
	
	// IA
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MiningAction;

	// 채굴 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MiningMontage;

	// 채굴 시 화면에 보일 곡괭이 메시입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickaxe")
	UStaticMeshComponent* PickaxeMesh;

	// 채굴 관련 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMiningComponent* MiningComponent;
	
/* 아이템 드랍 관련 변수 */
protected:
	// IMC
	// DropItemAction은 WorkMappingContext에서 매핑합니다.

	// IA
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* DropItemAction;
};
