// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LYJController.generated.h"

// 2025.04.06 LYJPlayerController 생성(추후 컨트롤러 병합 예정)

UCLASS()
class MBFACTORIO_API ALYJController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

    // UI 커서 클래스 (에디터에서 지정 가능하도록)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    TSubclassOf<class UMBFCursorWidget> CursorWidgetClass;

private:
    // 생성된 UI 커서 인스턴스
    UPROPERTY()
    class UMBFCursorWidget* CursorWidget;
};
