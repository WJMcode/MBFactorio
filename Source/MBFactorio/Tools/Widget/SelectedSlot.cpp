// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Widget/SelectedSlot.h"
#include "Components/Button.h"
#include "Styling/SlateBrush.h"

void USelectedSlot::NativeConstruct()
{
	Super::NativeConstruct();

	FString SlotName = FString::Printf(TEXT("Button"));
	FName WidgetName(*SlotName);

	// �̸����� ���� ã��
	UWidget* FoundWidget = GetWidgetFromName(WidgetName);
	if (FoundWidget)
	{
		UButton* SlotWidget = Cast<UButton>(FoundWidget);
		if (SlotWidget)
		{
			MyButton = SlotWidget;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("'%s' �� UMBFSlot�� �ƴմϴ�."), *SlotName);
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("'%s' ������ ã�� �� �����ϴ�."), *SlotName);
	}
	if (MyButton)
	{
		MyButton->OnClicked.AddDynamic(this, &ThisClass::Selected);
	}
}

void USelectedSlot::Selected()
{
	if (DownBorder)
	{
		FLinearColor Color = DownBorder->GetBrushColor();

		Color.A = 0.0f;


	}
	if (Back)
	{
		
		FSlateBrush Brush = Back->Brush;
		Brush.TintColor = FSlateColor(FLinearColor(0.051269, 0.049707, 0.051269, 1.0f)); // 초록색
		Back->SetBrush(Brush);
	}
	OnSlotClicked.Broadcast(SlotID);
}

void USelectedSlot::UnSelected()
{
	if (DownBorder)
	{
		FLinearColor Color = DownBorder->GetBrushColor();

		Color.A = 1.0f;


	}
	if (Back)
	{
		FSlateBrush Brush = Back->Brush;
		Brush.TintColor = FSlateColor(FLinearColor(0.283149, 0.274677, 0.274677, 1.0f)); // 초록색
		Back->SetBrush(Brush);
	}
}

