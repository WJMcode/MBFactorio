// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MiningComponent.generated.h"

class AResourceTile;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMiningProgress, float, Progress);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMiningComplete, EResourceType, Resource);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMiningStopped);

/*
 * 채굴 로직을 실행할 채굴 컴포넌트입니다.
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MBFACTORIO_API UMiningComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// 현재 캐릭터와 오버랩된 타일을 설정하는 함수입니다.
	void SetCurrentTargetTile(AResourceTile* InResourceTile);
	
	// 캐릭터의 채굴 가능 여부를 변경하는 함수
	void SetCanMine(bool CanMine);
	
	// 캐릭터의 채굴 상태를 변경하는 함수
	void SetIsMining(bool IsMining);

	// 캐릭터의 채굴 상태를 변경하는 함수
	void SetMiningAnimationPlaying(bool bPlaying);

public:
	FORCEINLINE const bool IsMining() const { return bIsMining; }

private:
	// 채굴 대상이 있는 방향으로 캐릭터를 회전시킵니다.
	void RotateToMiningTarget();

public:
	// 우클릭을 유지하여 채굴 시도
	void TryStartMining();
	// 채굴을 시작하는 함수
	void StartMining();
	// 우클릭 떼어 채굴을 멈춥니다.
	void StopMining();


protected:
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
	float MiningProgressValue = 0.0f;

	// 채굴 완료까지 걸리는 시간입니다.
	UPROPERTY(EditAnywhere)
	float MiningTimeToComplete = 1.8f;

public:
	// 채굴이 시작되면 HUD에게 알려 UMiningInteractionWidget의 ProgressBar를 업데이트합니다.
	UPROPERTY(BlueprintAssignable)
	FOnMiningProgress OnMiningProgress;

	// 채굴이 완료되면 HUD에게 알려 UMiningInteractionWidget의 CompleteText를 출력합니다.
	UPROPERTY(BlueprintAssignable)
	FOnMiningComplete OnMiningComplete;

	// 채굴이 중지되면 HUD에게 알려 UMiningInteractionWidget을 숨깁니다. 
	UPROPERTY(BlueprintAssignable)
	FOnMiningStopped OnMiningStopped;
};
