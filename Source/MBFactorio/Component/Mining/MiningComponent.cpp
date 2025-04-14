// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/Mining/MiningComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tools/MBFHUD.h"
#include "Character/PlayerCharacter.h"
#include "Component/MBFInventoryComponent.h"
#include "Tiles/TileTypes/ResourceTile.h"

void UMiningComponent::SetCurrentTargetTile(AResourceTile* InResourceTile)
{
	CurrentTargetTile = InResourceTile;
}

void UMiningComponent::SetCanMine(bool CanMine)
{
	bCanMine = CanMine;
}

void UMiningComponent::SetIsMining(bool IsMining)
{
	bIsMining = IsMining;
}

void UMiningComponent::SetMiningAnimationPlaying(bool bPlaying)
{
	bIsMiningAnimationPlaying = bPlaying;
}

void UMiningComponent::RotateToMiningTarget()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UMiningComponent::RotateToMiningTarget(): Owner가 nullptr입니다 !"));
		return;
	}

	if (!CurrentTargetTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentTargetTile이 없습니다!"));
		return;
	}

	FVector TargetLocation = CurrentTargetTile->GetActorLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), TargetLocation);
	// Z축(Yaw)만 회전
	LookAtRotation.Pitch = 0.f;
	LookAtRotation.Roll = 0.f;

	PlayerCharacter->SetActorRotation(LookAtRotation);
}

void UMiningComponent::TryStartMining()
{
	// 캐릭터와 오버랩된 타일이 없거나 채굴 불가능한 상태이면 return
	if (!CurrentTargetTile || !bCanMine) return;

	// 우클릭을 쭉 누르면 MiningHoldTime가 점점 증가
	MiningHoldTime += GetWorld()->GetDeltaSeconds();

	// MiningHoldTime가 MinHoldTimeToPlayAnim보다 크거나 같으면 채굴 시작
	// 즉 채굴 모션 시작 시간입니다. (광클 모션 방지)
	if (MiningHoldTime >= MinHoldTimeToPlayAnim)
	{
		// 캐릭터가 채굴 중이 아닐 때에만 타겟(오버랩된) 광물로 회전
		if (!IsMining())
		{
			// CurrentTargetTile이 있는 방향으로 캐릭터를 회전
			RotateToMiningTarget();
		}

		StartMining();
	}
}

void UMiningComponent::StartMining()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UMiningComponent::StartMining(): Owner가 nullptr입니다 !"));
		return;
	}

	// 플레이어가 채굴 중이 아니라면
	if (!IsMining())
	{
		SetIsMining(true);

		PlayerCharacter->ShowPickaxe(true);
	}

	// 채굴 시작
	{
		 /* 프로그래스바를 채우기 위해 채굴이 시작되면 HUD에게 Broadcast합니다.
			 프로그래스바는 0.f ~ 1.f의 값을 받아 바를 채웁니다.					*/
		OnMiningProgress.Broadcast(MiningProgressValue / MiningTimeToComplete);

		// 채굴 진행바를 채우기 위한 변수의 값을 점점 올림
		MiningProgressValue += GetWorld()->GetDeltaSeconds();

		// 채굴 완료
		if (MiningProgressValue >= MiningTimeToComplete)
		{
			MiningProgressValue = 0.f;

			// 채굴한 광물을 텍스트로 출력하기 위해 HUD에게 Broadcast합니다.
			OnMiningComplete.Broadcast(CurrentTargetTile->GetResourceType());
			
			// 인벤토리에 채굴한 아이템 삽입
			{
				EResourceType MinedResourceType = CurrentTargetTile->GetResourceType();
				FName ItemName = NAME_None;
				switch (MinedResourceType)
				{
				case EResourceType::Copper:
					ItemName = TEXT("7");
					break;
				case EResourceType::Iron:
					ItemName = TEXT("4");
					break;
				case EResourceType::Stone:
					ItemName = TEXT("10");
					break;
				case EResourceType::Coal:
					ItemName = TEXT("3");
					break;
				case EResourceType::Unknown:
					UE_LOG(LogTemp, Error, TEXT("MinedResourceType: 알 수 없는 타입니다 !"));
					break;
				default:
					UE_LOG(LogTemp, Warning, TEXT("MinedResourceType: 처리되지 않은 타입입니다."));
					break;
				}
				if (ItemName != NAME_None)
				{
					PlayerCharacter->GetInventoryComponent()->AddItem(ItemName, 1);
				}
			}
		}
	}

	// 채굴 몽타주가 재생되고 있지 않다면, 채굴 몽타주 재생
	if (!bIsMiningAnimationPlaying)
	{
		PlayerCharacter->PlayMiningAnimation();
	}
}

void UMiningComponent::StopMining()
{
	APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(GetOwner());
	if (!PlayerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("UMiningComponent::StopMining(): Owner가 nullptr입니다 !"));
		return;
	}

	// 플레이어가 채굴 중이라면
	if (IsMining())
	{
		SetIsMining(false);

		PlayerCharacter->ShowPickaxe(false);
	}

	// 채굴 정지
	{
		OnMiningStopped.Broadcast();
		
		// 채굴 진행바와 우클릭 누적 시간을 0으로
		MiningProgressValue = 0.0f;

		MiningHoldTime = 0.0f;
	}

	// 채굴 몽타주가 재생되고 있다면 몽타주를 멈춤.
	if (bIsMiningAnimationPlaying)
	{
		PlayerCharacter->StopMiningAnimation();
	}
}