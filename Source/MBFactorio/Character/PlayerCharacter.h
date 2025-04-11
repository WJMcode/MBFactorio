#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "PlayerCharacter.generated.h"

//@TODO: 
//		 채굴 진행바 표시
//		 채굴 완료 시 아이템 습득 텍스트 출력
//		 채굴한 광물 인벤토리에 추가
//		 채굴 기능 컴포넌트로 분리

// 이슈:
// 채굴 모션 자체가 움직이는 모션이라 루트를 잠금 -> 방향에 따라 모션이 달라지는 문제 발생

class UInputComponent;
class AResourceTile;

/*
 * 플레이어가 조종하는 캐릭터입니다. 
 * APlayerCharacter에서 채굴 관련 동작을 시작합니다.
 */
UCLASS()
class MBFACTORIO_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	friend class AWJMController;
	friend class ALYJController;

public:
	APlayerCharacter();
	
	// 디버깅용
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:
	// 플레이어 입력을 처리하기 위해 호출되는 함수
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	// 캐릭터 이동을 위한 함수
	void MoveCharacter(const FInputActionValue& Value);
	
/* 채굴 관련 함수 */
public:
	// 현재 캐릭터와 오버랩된 타일을 설정하는 함수입니다.
	void SetCurrentTargetTile(AResourceTile* InResourceTile);
	
	// 캐릭터의 채굴 가능 여부를 변경하는 함수
	void SetCanMine(bool CanMine);
	
	// 캐릭터의 채굴 상태를 변경하는 함수
	void SetIsMining(bool IsMining);

public:
	const bool GetIsMining() const { return bIsMining; }

private:
	// 채굴 대상이 있는 방향으로 캐릭터를 회전시킵니다.
	void RotateToMiningTarget();

	// 우클릭을 유지하여 채굴 시도
	void TryStartMining();
	// 채굴을 시작하는 함수
	void StartMining();
	// 우클릭 떼어 채굴을 멈춥니다.
	void StopMining();

	// 곡괭이 메시를 화면에 보이거나 숨깁니다.
	void ShowPickaxe(bool bVisible);

private:
	// 채굴 몽타주 재생
	void PlayMiningAnimation();
	// 채굴 몽타주를 멈춥니다.
	void StopMiningAnimation();	
	
	// 채굴 몽타주가 다 재생되면 자동으로 호출되는 함수입니다.
	UFUNCTION()
	void OnMiningMontageEnded(UAnimMontage* Montage, bool bInterrupted);

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

	// 캐릭터와 오버랩된 타일을 저장합니다.
	AResourceTile* CurrentTargetTile = nullptr;

	// 캐릭터가 채굴 가능한 상태인지 나타냅니다.
	bool bCanMine = false;

	// 현재 캐릭터가 채굴 중인지 구분하는 변수
	bool bIsMining = false;

	// 채굴 몽타주의 재생 여부를 나타냅니다.
	bool bIsMiningAnimationPlaying = false;

	// 우클릭을 유지한 누적 시간입니다.
	float MiningHoldTime = 0.f;

	// MinHoldTimeToPlayAnim 값만큼 우클릭을 유지해야 채굴을 시작합니다.
	const float MinHoldTimeToPlayAnim = 0.2f;

	// 채굴 진행바를 표현할 변수입니다.
	float MiningProgress = 0.0f;

	// 채굴 완료까지 걸리는 시간입니다.
	UPROPERTY(EditAnywhere)
	float MiningTimeToComplete = 2.0f;
};
