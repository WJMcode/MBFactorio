#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputMappingContext.h"
#include "Component/MBFInventoryComponent.h"
#include "Component/CraftComponent.h"
#include "PlayerCharacter.generated.h"

//@TODO: 
//		 채굴 완료 시, 채굴 개수 출력
//		 채굴한 광물 인벤토리에 추가

// 이슈:
//		채굴 모션 자체가 움직이는 모션이라 루트를 잠금 -> 방향에 따라 모션이 달라지는 문제
//		게임 시작하자마자 밟은 광물은 인식이 안됨

class UInputComponent;
class UMiningComponent;

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
	// 플레이어 입력을 처리하기 위해 호출되는 함수
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

protected:
	// 캐릭터 이동을 위한 함수
	void MoveCharacter(const FInputActionValue& Value);

// ======= 인벤토리 관련 함수 =======
public:
	UMBFInventoryComponent* GetInventoryComponent() const { return InventoryComp; }

	UCraftComponent* GetCraftComponent() const { return CraftComp; }

// ======= 채굴 관련 함수 =======
public:
	UMiningComponent* GetMiningComponent() const { return MiningComponent; }

public:
	// 곡괭이 메시를 화면에 보이거나 숨깁니다.
	void ShowPickaxe(bool bVisible);

public:
	// 채굴 몽타주 재생
	void PlayMiningAnimation();
	// 채굴 몽타주를 멈춥니다.
	void StopMiningAnimation();	
	
private:
	// 채굴 몽타주가 다 재생되면 자동으로 호출되는 함수입니다.
	UFUNCTION()
	void OnMiningMontageEnded(UAnimMontage* Montage, bool bInterrupted);

// ======= 플레이어 움직임 관련 변수 ======= 
protected:
	// IMC
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* MoveMappingContext;
	
	// IA
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MoveAction;
// =======  인벤토리 관련 변수 =======
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMBFInventoryComponent* InventoryComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCraftComponent* CraftComp;

// =======  채굴 관련 변수 =======
protected:
	// 채굴 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
	UAnimMontage* MiningMontage;

	// 채굴 시 화면에 보일 곡괭이 메시입니다.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Pickaxe")
	UStaticMeshComponent* PickaxeMesh;

	// 채굴 관련 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UMiningComponent* MiningComponent;
};
