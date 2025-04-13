// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Component/MBFInventoryComponent.h"
#include "LYJController.generated.h"

// WJMController.h의 코드를 복사한 상태입니다.
// MBFController.h의 코드를 복사한 상태입니다.

class UInputAction;
class UInputMappingContext;

class UGameHUD;
class UGameMenuWidget;
class UMBFCursorWidget;
class UReplayMenuWidget;
class UItemCursorWidget;

class AResourceTile;

UCLASS()
class MBFACTORIO_API ALYJController : public APlayerController
{
    GENERATED_BODY()

public:
    ALYJController();

protected:
    virtual void BeginPlay() override;                      // 게임 시작 시 초기화
    virtual void Tick(float DeltaTime) override;           // 매 프레임 호출
    virtual void SetupInputComponent() override;           // 입력 바인딩 설정

public:
    AResourceTile* GetDetectedStope() const { return DetectedStope; } // 현재 감지된 광물 반환
    UMBFInventoryComponent* GetInventoryComponent() { return MBFInventoryComponent; } // MBFController에서 복사

    void CreateAndAddCursorWidget();                       // CursorWidget 생성 및 Viewport 등록
    void SetGameOnlyInput();                               // 입력 모드 - 게임 전용
    void SetGameAndUIInput();                              // 입력 모드 - 게임 + UI

    void UpdateCursorVisibility(AResourceTile* InStope);   // 커서 색상 및 표시 상태 업데이트
    void GameHUD();                                        // GameHUD 위젯 생성 및 추가
    void ToggleGameMenu();                                 // 게임 메뉴 UI 표시/숨기기 토글
    void OpenGameMenu();                                   // 게임 메뉴 UI 열기
    void CloseGameMenu();                                  // 게임 메뉴 UI 닫기
    void OpenReplayMenu();                                 // 리플레이 메뉴 UI 열기
    void OnGameMenuPressed();                              // 메뉴 단축키(Tab) 입력 시 호출
    void RecreateCursorWidget();                           // 커서 위젯 재생성
    
    void CheckVelocity();                                  // 캐릭터의 속도를 체크
    void FreezeCursorAndHideUI();                          // 커서 고정 및 UI 숨김
    void UnfreezeCursorAndShowUI();                        // 커서 복원 및 UI 표시

    void SetDetectedStope(AResourceTile* InStope);         // 감지된 광물 설정
    void SetPlayerNearStope(bool bNear);                   // 플레이어 근접 여부 설정
    AActor* FindOverlappingStope();                        // 플레이어 주변 ResourceTile 찾기

    void OnClickInventory();                               // 인벤토리 슬롯 클릭 시 커서 처리 함수

    void InventoryTogle(); // MBFController에서 복사

public:
    UFUNCTION(BlueprintCallable)
    UItemCursorWidget* GetItemCursorWidget();

private:
    void StopCharacterAction();                            // 캐릭터 채굴 중단

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

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* OpenInventory;

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
    bool bIsGameMenuOpen = false;       // 게임 메뉴 열림 여부
    bool bIsCursorOverStope = false;    // 커서가 광물 위에 있는지 여부
    bool bCursorActive = true;          // 현재 커서 활성 여부

private:
    UPROPERTY()
    AResourceTile* DetectedStope = nullptr; // 감지된 광물 캐시

    UMBFInventoryComponent* MBFInventoryComponent; // MBFController에서 복사
};

