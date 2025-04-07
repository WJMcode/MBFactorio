// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Struct/MBFStope.h"
#include "LYJController.generated.h"


UCLASS()
class MBFACTORIO_API ALYJController : public APlayerController
{
	GENERATED_BODY()
	
public:
    ALYJController();

protected:
	virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;

public:
    void UpdateCursorVisibility();
    void SetPlayerNearStope(bool bNear);

public:
    /** Cursor UI 위젯 클래스 (BP에서 할당) */
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TSubclassOf<class UMBFCursorWidget> CursorWidgetClass;

    /** 생성된 커서 위젯 */
    UPROPERTY()
    class UMBFCursorWidget* CursorWidget;

    bool bIsCursorOverStope = false;

};
