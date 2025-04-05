// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputMappingContext.h"
#include "WJMController.generated.h"

//@TODO: MBFController와 합치기 (원재민 - 작성 중)

/**
 * 플레이어 캐릭터의 컨트롤러입니다.
 * AWJMController에서 인벤토리 Widget을 생성 및 제거합니다.
 */
UCLASS()
class MBFACTORIO_API AWJMController : public APlayerController
{
	GENERATED_BODY()

protected:
	// 플레이어 입력을 처리하기 위해 호출되는 함수
	virtual void SetupInputComponent();
	
	virtual void BeginPlay() override;

protected:
	// 인벤토리를 열고 닫기 위한 함수
	void ToggleInventory();

private:
	void OpenInventory();
	void CloseInventory();
	
/* 플레이어 인벤토리 입력 관련 */
protected:	
	// 인벤토리 Widget을 저장하는 변수
	UUserWidget* InventoryWidget;

	/* 아래 멤버 변수들은 에디터에서 세팅합니다. */
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUserWidget> InventoryWidgetClass;
	
	/* IMC */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InventoryMappingContext;

	/* IA */
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* ToggleInventoryAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* CancelAction;
};
