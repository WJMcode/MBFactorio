// Fill out your copyright notice in the Description page of Project Settings.


#include "ResourceTile.h"

AResourceTile::AResourceTile()
{
	// 필요시 Tick 비활성화
	PrimaryActorTick.bCanEverTick = false;
}

//void AResourceTile::InitializeTile(float InTileSize)
//{
//	Super::InitializeTile(InTileSize);
//
//	if (ResourceMaterialSets.Num() == 0)
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AResourceTile: ResourceMaterialSets가 비어있습니다."));
//		return;
//	}
//
//	// 랜덤한 리소스 세트 선택
//	int32 SetIndex = FMath::RandRange(0, ResourceMaterialSets.Num() - 1);
//	const FResourceMaterialSet& SelectedSet = ResourceMaterialSets[SetIndex];
//
//	// 타입 설정
//	ResourceType = SelectedSet.ResourceType;
//
//	// 머티리얼 랜덤 선택
//	if (SelectedSet.Materials.Num() > 0)
//	{
//		int32 MaterialIndex = FMath::RandRange(0, SelectedSet.Materials.Num() - 1);
//		TileMesh->SetMaterial(0, SelectedSet.Materials[MaterialIndex]);
//	}
//	else
//	{
//		UE_LOG(LogTemp, Warning, TEXT("AResourceTile: 선택된 세트에 머티리얼이 없습니다."));
//	}
//}

void AResourceTile::SetResourceType(EResourceType InType)
{
	ResourceType = InType;
}