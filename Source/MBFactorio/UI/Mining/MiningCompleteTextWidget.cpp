// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Mining/MiningCompleteTextWidget.h"
#include "Components/TextBlock.h"

void UMiningCompleteTextWidget::SetMessageText(const FText& NewText)
{
    if (MessageTextBlock)
    {
        MessageTextBlock->SetText(NewText);
    }
}
