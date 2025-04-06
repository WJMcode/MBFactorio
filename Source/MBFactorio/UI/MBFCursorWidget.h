// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MBFCursorWidget.generated.h"

class UImage;

UCLASS()
class MBFACTORIO_API UMBFCursorWidget : public UUserWidget
{
	GENERATED_BODY()

public:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
    UPROPERTY(meta = (BindWidget))
    UImage* FrameCursor;

private:
    void UpdateCursorPosition();
    void UpdateCursorVisibilityAndColor();
	
};
