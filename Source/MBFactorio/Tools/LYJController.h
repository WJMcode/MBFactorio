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
    void SetPlayerNearStope(bool bNear);

    void ToggleGameMenu();

public:
    UPROPERTY(EditDefaultsOnly, Category = "Input")
    UInputMappingContext* InputMappingContext;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
    UInputAction* GameMenuAction;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<UGameMenuWidget> GameMenuWidgetClass;

    /** Cursor UI 위젯 클래스 (BP에서 할당) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UMBFCursorWidget> CursorWidgetClass;

    /** 생성된 커서 위젯 */
    UPROPERTY()
    class UMBFCursorWidget* CursorWidget;

protected:    
    UGameMenuWidget* GameMenuWidget;
    bool bIsGameMenuOpen = false;

    bool bIsCursorOverStope = false;

};
