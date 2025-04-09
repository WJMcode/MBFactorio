// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/MBFStope.h"
#include "LYJController.generated.h"

class UInputAction;
class UInputMappingContext;
class UGameMenuWidget;
class UMBFCursorWidget;
class UReplayMenuWidget;

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

    void OnGameMenuPressed();

public:
    void UpdateCursorVisibility();
    void ToggleGameMenu();
    void OpenReplayMenu();

public:
    void SetPlayerNearStope(bool bNear);

public:    
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
    UMBFCursorWidget* CursorWidget;

    UPROPERTY()
    UGameMenuWidget* GameMenuWidget;

    UPROPERTY()
    UReplayMenuWidget* ReplayMenuWidget;

public:       
    bool bIsGameMenuOpen = false;
    bool bIsCursorOverStope = false;
};
