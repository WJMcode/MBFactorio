// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "LYJCharacter.generated.h"

UCLASS()
class MBFACTORIO_API ALYJCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALYJCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
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
