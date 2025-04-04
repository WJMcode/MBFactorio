// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 플레이어 캐릭터에 적용할 AnimInstance입니다.
 */
UCLASS()
class MBFACTORIO_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	// 애니메이션 업데이트 이벤트 (Tick)
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	// 플레이어 속도에 따라 Idle 또는 Run 모션을 출력
	UPROPERTY(BlueprintReadOnly, Category = "Character")
	float Speed;
};
