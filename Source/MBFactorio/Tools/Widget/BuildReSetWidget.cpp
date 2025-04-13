// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/BuildReSetWidget.h"
#include "test/CraftMachine.h"

FReply UBuildReSetWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{


		if (OwnerActor)
		{
			OwnerActor->ResetBuildItem();
		}

		return FReply::Handled();
	}

	return FReply::Unhandled();
}
