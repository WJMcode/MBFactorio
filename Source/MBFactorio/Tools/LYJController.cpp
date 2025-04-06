// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/LYJController.h"
#include "UI/MBFCursorWidget.h"

void ALYJController::BeginPlay()
{
    Super::BeginPlay();
    
    // MBFCursorWidget에서 이미지 적용 잘 안 될 시 활용할 것
    CursorWidgetClass = LoadClass<UMBFCursorWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/UI/UI_Cursor.UI_Cursor_C'"));

    if (CursorWidgetClass)
    {
        // 위젯 생성
        CursorWidget = CreateWidget<UMBFCursorWidget>(this, CursorWidgetClass);
        if (CursorWidget)
        {
            // Viewport에 추가
            CursorWidget->AddToViewport();
        }
    }

    // 입력 모드 Game Only로 설정
    FInputModeGameOnly InputMode;
    SetInputMode(InputMode);

    // 마우스 커서 표시
    bShowMouseCursor = true;
}
