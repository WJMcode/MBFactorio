// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/MBFStope.h"
#include "LYJController.generated.h"

// WJMController.h의 코드를 복사한 상태입니다.

class UInputAction;
class UInputMappingContext;

class UGameHUD;
class UGameMenuWidget;
class UMBFCursorWidget;
class UReplayMenuWidget;

class AResourceTile;

UCLASS()
class MBFACTORIO_API ALYJController : public APlayerController
{
	GENERATED_BODY()

public:
    ALYJController();

protected:
	virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupInputComponent() override;

public:
    // 최근에 플레이어가 감지한 광물을 반환합니다.
    AResourceTile* GetDetectedStope() const { return DetectedStope; }

    //void UpdateCursorVisibility(); // MBFStope 감지 시

    void UpdateCursorVisibility(AResourceTile* InStope);
    void GameHUD();
    void ToggleGameMenu();
    void OpenGameMenu();
    void CloseGameMenu();
    void OpenReplayMenu();
    void OnGameMenuPressed();
    void RecreateCursorWidget();

public:
    // 캐릭터가 감지한 광물을 저장합니다.
    void SetDetectedStope(AResourceTile* InStope);
    void SetPlayerNearStope(bool bNear);

private:
    // 캐릭터의 채굴 동작을 멈춥니다.
    void StopCharacterAction();

public:    
    /** GameHUD 위젯 클래스 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UGameHUD> GameHUDClass;

    /** Cursor UI 위젯 클래스 (BP에서 할당) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UMBFCursorWidget> CursorWidgetClass;

    /** GameMenu UI 위젯 클래스 */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UGameMenuWidget> GameMenuWidgetClass;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UReplayMenuWidget> ReplayMenuWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* IMC_Menu;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* GameMenuAction;

protected:
    /** 생성된 커서 위젯 */
    UPROPERTY()
    UGameHUD* GameHUDWidget;

    UPROPERTY()
    UMBFCursorWidget* CursorWidget;

    UPROPERTY()
    UGameMenuWidget* GameMenuWidget;

    UPROPERTY()
    UReplayMenuWidget* ReplayMenuWidget;

public:       
    bool bIsGameMenuOpen = false;
    bool bIsCursorOverStope = false;

private:
    // 캐릭터가 최근 감지한 광물
    UPROPERTY()
    AResourceTile* DetectedStope = nullptr;
};
