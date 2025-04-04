// Fill out your copyright notice in the Description page of Project Settings.

#include "Animations/PlayerAnimInstance.h"

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    // 캐릭터 속도 가져오기
    APawn* OwnerPawn = TryGetPawnOwner();
    if (OwnerPawn)
    {
        Speed = OwnerPawn->GetVelocity().Size();
    }
}

