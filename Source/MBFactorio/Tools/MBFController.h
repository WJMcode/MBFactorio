// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Component/CraftComponent.h"
#include "Struct/MBFStruct.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Tools/MBFHUD.h"
#include "Misc/Misc.h"
#include "Tiles/TileTypes/StructuresTile.h"
#include "Character/PlayerCharacter.h"
#include "Component/MBFInventoryComponent.h"
#include "Test/TestActor.h"
#include "Test/CraftMachine.h"
#include "MBFController.generated.h"

class UGameHUD;
class UMBFCursorWidget;
class UGameMenuWidget;
class UReplayMenuWidget;
class UItemCursorWidget;
class UFurnaceInventory;

class AResourceTile;

/**
 *
 */
UCLASS()
class MBFACTORIO_API AMBFController : public APlayerController
{
	GENERATED_BODY()

	UInputMappingContext* IMC_Default;

	AMBFController();
	
public:
	UMBFInventoryComponent* GetInventoryComponent() { return MBFInventoryComponent; }
	UCraftComponent* GetCraftComponent() { return CraftComponent; }

	void SetFurnaceOwner(ATestActor* InActor) { FurnaceUIOwnerActor = InActor; }
	ATestActor* GetFurnaceOwner() { return FurnaceUIOwnerActor; }

	void SetCraftMachineOwner(ACraftMachine* InActor) { CraftMachineUIOwnerActor = InActor; }
	ACraftMachine* GetCraftMachineOwner() { return CraftMachineUIOwnerActor; }

	bool bOpenInventory;

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
	virtual void Tick(float DeltaSeconds) override;

public:
	void GameHUD();                                        // GameHUD 위젯 생성 및 추가
	void CreateAndAddCursorWidget();                       // CursorWidget 생성 및 Viewport 등록
	void RecreateCursorWidget();                           // 커서 위젯 재생성
	void SetGameAndUIInput();                              // 입력 모드 - 게임 + UI
	void SetGameOnlyInput();                               // 입력 모드 - 게임 전용

public:
	UFUNCTION(BlueprintCallable)
	UItemCursorWidget* GetItemCursorWidget();

	void UpdateCursorVisibility(AResourceTile* InStope);   // 커서 색상 및 표시 상태 업데이트
	void FreezeCursorAndHideUI();                          // 커서 고정 및 UI 숨김
	void UnfreezeCursorAndShowUI();                        // 커서 복원 및 UI 표시
	
public:
	void CheckVelocity();                                  // 캐릭터의 속도를 체크

public:
	void OpenReplayMenu();                                 // 리플레이 메뉴 UI 열기
	void ToggleGameMenu();                                 // 게임 메뉴 UI 표시/숨기기 토글

private:
	void OnGameMenuPressed();                              // 메뉴 단축키(Tab) 입력 시 호출
	void OnClickInventory();                               // 인벤토리 슬롯 클릭 시 커서 처리 함수

	void OpenGameMenu();                                   // 게임 메뉴 UI 열기
	void CloseGameMenu();                                  // 게임 메뉴 UI 닫기
	
	void InventoryTogle();
	
public:
	void SetPlayerNearObject(bool bNear);                   // 플레이어 근접 여부 설정
	
	// 캐릭터와 오버랩된 Tile들 중 첫 번째 요소를 반환하는 함수입니다.
	template<typename T>
	AActor* FindOverlappingTile()
	{
		static_assert(TIsDerivedFrom<T, AActor>::IsDerived, "T 는 AActor를 상속받은 클래스이어야 합니다.");

		TArray<AActor*> OverlappingActors;

		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetPawn());
		if (!PlayerCharacter)
		{
			UE_LOG(LogTemp, Error, TEXT("AWJMController::FindOverlappingTile(): PlayerCharacter가 nullptr입니다!"));
			return nullptr;
		}
		// 캐릭터와 오버랩된 Tile들을 찾아 OverlappingActors에 저장
		PlayerCharacter->GetOverlappingActors(OverlappingActors, T::StaticClass());

		if (OverlappingActors.IsEmpty())
		{
			return nullptr;
		}
		// 오버랩된 ResourceTile들 중 첫 번째 요소를 반환
		return Cast<T>(OverlappingActors[0]);
	}

// ============ 아이템 드랍 관련 함수 ============
private:
	// Z를 눌러 아이템 드랍
	void OnDropItemStarted();

// ============ 채굴 관련 함수 ============
public:
	// 최근에 캐릭터가 감지한 광물을 반환합니다.
	AResourceTile* GetDetectedStope() const { return DetectedStope; }

public:
	// 캐릭터가 감지한 광물을 저장합니다.
	void SetDetectedStope(AResourceTile* InStope);

private:
	// 우클릭을 유지하여 채굴 시도
	void OnMiningTriggered();
	// 우클릭 떼어 채굴을 멈춥니다.
	void OnMiningReleased();
	
private:
	void StopCharacterAction();                            // 캐릭터 채굴 중단

// ============ 구조물 관련 함수 ============
private:
	// CursorWidget 생성 및 Viewport 등록
	void CreateStructuresWidget();

public:
	// 최근에 캐릭터가 감지한 광물을 반환합니다.
	AStructuresTile* GetDetectedStructures() const { return DetectedStructures; }

public:
	void SetCanOpenStructuresUI(bool bCanOpen);
	// 캐릭터가 감지한 구조물을 저장합니다.
	void SetDetectedStructures(AStructuresTile* InStructures);

private:
	// 구조물을 좌클릭하여 UI를 엽니다.
	void OpenStructuresUI();
	// ESC를 눌러 UI를 닫습니다.
	void CloseStructuresUI();

// ============ 기타 함수 ============
private:
	/*  E키는 구조물 UI가 켜져 있는 경우 이를 닫거나,
		인벤토리를 열고 닫는 역할을 합니다.                    */
	void OnEKeyPressed();

public:
	// -------------------- 인풋 관련 --------------------
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_Menu;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* GameMenuAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_ClickInventory;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* ClickAction;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* IMC_Inventory;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	//UInputAction* OpenInventory;

protected:
	// -------------------- 인풋 관련 (아이템 드랍) --------------------
	// IMC
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* WorkMappingContext;

	// IA
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* DropItemAction;

protected:
	// -------------------- 인풋 관련 (채굴) --------------------
	// IMC
	// MiningAction은 WorkMappingContext에서 매핑합니다.

	// IA
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* MiningAction;

public:
	// -------------------- UI 관련 --------------------
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UGameHUD> GameHUDClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UMBFCursorWidget> CursorWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UGameMenuWidget> GameMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UReplayMenuWidget> ReplayMenuWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
	TSubclassOf<UItemCursorWidget> ItemCursorWidgetClass;

protected:
	UPROPERTY()
	UGameHUD* GameHUDWidget;

	UPROPERTY()
	UMBFCursorWidget* CursorWidget;

	UPROPERTY()
	UGameMenuWidget* GameMenuWidget;

	UPROPERTY()
	UReplayMenuWidget* ReplayMenuWidget;

	UPROPERTY()
	UItemCursorWidget* ItemCursorWidget;

public:
	bool bCursorActive = true;          // 현재 커서 활성 여부
	bool bIsGameMenuOpen = false;       // 게임 메뉴 열림 여부
	bool bIsCursorOverObject = false;    // 커서가 광물이나 구조물 등 오브젝트 위에 있는지 여부

private:
	UMBFInventoryComponent* MBFInventoryComponent;
	UCraftComponent* CraftComponent;
	ATestActor* FurnaceUIOwnerActor;
	ACraftMachine* CraftMachineUIOwnerActor;

private:
	UPROPERTY()
	AResourceTile* DetectedStope = nullptr; // 감지된 광물 캐시

// ============ 구조물 관련 변수 ============
protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UFurnaceInventory> StructureInteractionWidgetClass;

	// 구조물 Widget을 저장하는 변수
	UPROPERTY()
	UFurnaceInventory* StructureInteractionWidget;
	
	/*** IMC ***/
	// UI 상호작용 IMC입니다.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* InteractionUIMappingContext;
	
	/*** IA ***/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* OpenStructureUIAction;

private:
	// 구조물 UI 오픈 가능 상태를 나타내는 변수입니다.
	bool bCanOpenStructuresUI = false;

	// 구조물 UI가 오픈 상태인지 나타냅니다.
	bool bOpenStructureUI = false;

	// 캐릭터가 최근 감지한 구조물
	UPROPERTY()
	AStructuresTile* DetectedStructures = nullptr;

// ============ 기타 관련 변수 ============
protected:
	// IMC
	// InteractOrToggleUIAction은 InteractionUIMappingContext에서 매핑합니다.

	/*** IA ***/
	// 구조물 UI를 끄거나 인벤토리를 열고 닫습니다.
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputAction* InteractOrToggleUIAction;
};
