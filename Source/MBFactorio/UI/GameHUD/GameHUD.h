// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Tiles/TileBase/TileStructs.h" 
#include "GameHUD.generated.h"

// @TODO 좌측 상단: 목표(타이틀 + 내용(수정 가능하게))
// @TODO 우측 상단: 오브젝트 정보(이미지 + 오브젝트명 + 정보)
// @TODO 중앙 하단: 퀵 인벤토리(드래그앤드롭 가능하게)

class UImage;
class UTextBlock;
class UTexture2D;

class UUniformGridPanel;
class UHorizontalBox;
class UMBFSlot;
class UCraftSelectSlot;

// @TODO 인벤토리 정보와 연결지어 DrillCount, FurnaceCount 변수에 저장해야 함.

UCLASS()
class MBFACTORIO_API UGameHUD : public UUserWidget
{
    GENERATED_BODY()

public:
    // 진행 상황 텍스트를 갱신하는 함수
    UFUNCTION(BlueprintCallable)
    void UpdateGoalStatus(int32 DrillCount, int32 FurnaceCount);

    // 리소스 정보 UI 업데이트
    UFUNCTION(BlueprintCallable)
    void UpdateResourceInfo(EResourceType ResourceType);

    // 퀵 인벤토리 슬롯
    UFUNCTION(BlueprintCallable)
    void UpdateInventorySlot(int32 SlotIndex, FName ItemID, int32 Count);
    
protected:
    virtual void NativeConstruct() override;

protected:
    /** 좌측 상단 목표*/
    // 목표 텍스트 박스에 연결된 위젯 (UMG에서 바인딩 필요)
    UPROPERTY(meta = (BindWidget))
    UTextBlock* GoalText;

protected:
    /** 우측 상단 오브젝트 정보*/
    // 바인딩된 위젯 (UMG에서 이름 매칭 필수)
    UPROPERTY(meta = (BindWidget))
    UImage* ObjectImage;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ObjectType;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* ObjectInfo;

protected:
    /** 중앙 하단 퀵 인벤토리*/
    UPROPERTY(meta = (BindWidget))
    UUniformGridPanel* ItemSlotsPanel;

    UPROPERTY(meta = (BindWidget))
    UHorizontalBox* SelectedSlotsBox;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UMBFSlot> SlotWidgetClass;

    UPROPERTY(EditDefaultsOnly, Category = "UI")
    TSubclassOf<UCraftSelectSlot> SelectedSlotClass;

protected:
    // 내부에서 사용할 리소스 정보 맵
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "UI")
    TMap<EResourceType, UTexture2D*> ResourceImages;

    UPROPERTY(EditAnywhere, Category = "UI")
    TMap<EResourceType, FString> ResourceDescriptions;

    // 내부 텍스트 생성 함수
    FString GetResourceType(EResourceType Type) const;

private:
    UPROPERTY()
    TArray<UMBFSlot*> ItemSlots;

    UPROPERTY()
    TArray<UCraftSelectSlot*> SelectedSlots;

    void CreateSlots();
};
