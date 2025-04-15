// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/BuildReSetWidget.h"
#include "Tiles/TileMachine/CraftMachineTile.h"

FReply UBuildReSetWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{


		if (OwnerTile)
		{
			OwnerTile->ResetBuildItem();
		}

		return FReply::Handled();
	}

	return FReply::Unhandled();
}
