#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Tiles/TileTypes/ResourceTile.h"
#include "UI/MiningInterationWidget.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터가 이동 방향으로 회전하도록 합니다.
	{
		bUseControllerRotationYaw = false; // 컨트롤러 Yaw 회전 비활성화
	}

	PickaxeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickaxeMesh"));
	PickaxeMesh->SetupAttachment(GetMesh(), TEXT("PickaxeSocket")); // 소켓 이름
	PickaxeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShowPickaxe(false); // 곡괭이가 기본적으로 안 보이게 설정
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GEngine)
	{
		FString Msg = FString::Printf(TEXT("MiningProgress Value: %.1f"), MiningHoldTime);
		GEngine->AddOnScreenDebugMessage(1, 0.f, FColor::Emerald, Msg); // ID 1로 고정해서 갱신됨
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
		{
			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
			{
				// IMC 등록
				Subsystem->AddMappingContext(MoveMappingContext, 0);
				Subsystem->AddMappingContext(WorkMappingContext, 0);
			}
		}
	}

	if (UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance())
	{
		/* 현재 재생 중인 몽타주의 재생이 끝나면 
		OnMiningMontageEnded 함수를 호출하도록델리게이트 바인딩 */
		AnimInstance->OnMontageEnded.AddDynamic(this, &APlayerCharacter::OnMiningMontageEnded);
	}

	if (MiningInterationWidgetClass) // UPROPERTY로 에디터에서 지정
	{
		MiningInterationWidget = CreateWidget<UMiningInterationWidget>(GetWorld(), MiningInterationWidgetClass);
		if (MiningInterationWidget)
		{
			MiningInterationWidget->AddToViewport();
			MiningInterationWidget->SetVisibility(ESlateVisibility::Hidden); // 시작은 숨김 처리 (선택)
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input용 컴포넌트로 캐스팅
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		/* InputAction을 C++ 함수에 바인딩 */
		// 채굴 관련 액션
		EnhancedInput->BindAction(MiningAction, ETriggerEvent::Triggered, this, &APlayerCharacter::TryStartMining);
		EnhancedInput->BindAction(MiningAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopMining);

		// 캐릭터 움직임 관련 액션
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveCharacter);
	}
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& Value)
{
	// 채굴 중에 움직이면 채굴을 멈춤
	if (bIsMining)
	{
		StopMining();
	}

	FVector2D InputVector = Value.Get<FVector2D>();

	if (Controller && !InputVector.IsNearlyZero())
	{
		// 회전 기준은 그대로 카메라 방향 유지
		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// 최종 이동 방향 계산
		FVector MoveDir = (Forward * InputVector.Y + Right * InputVector.X).GetSafeNormal();

		// 이동
		AddMovementInput(MoveDir);

		// 방향키를 누르면 캐릭터 즉시 회전
		SetActorRotation(MoveDir.Rotation());
	}
}

void APlayerCharacter::RotateToMiningTarget()
{
	if (!CurrentTargetTile)
	{
		UE_LOG(LogTemp, Warning, TEXT("CurrentTargetTile이 없습니다!"));
		return;
	}
	
	FVector TargetLocation = CurrentTargetTile->GetActorLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetLocation);
	// Z축(Yaw)만 회전
	LookAtRotation.Pitch = 0.f;
	LookAtRotation.Roll = 0.f;

	SetActorRotation(LookAtRotation);
}

void APlayerCharacter::TryStartMining()
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
		if(!GetIsMining())
		{
			// CurrentTargetTile이 있는 방향으로 캐릭터를 회전
			RotateToMiningTarget();
		}

		StartMining();
	}
}

void APlayerCharacter::StartMining()
{
	// 플레이어가 채굴 중이 아니라면
	if (!GetIsMining())
	{
		SetIsMining(true);

		ShowPickaxe(true);
	}
	
	// 채굴 시작
	{
		if (MiningInterationWidget)
		{
			MiningInterationWidget->SetVisibility(ESlateVisibility::Visible);
			MiningInterationWidget->MiningCompleteText->SetVisibility(ESlateVisibility::Hidden);
		}

		// 채굴 진행바 업데이트
		MiningInterationWidget->SetMiningProgress(MiningProgressValue / MiningTimeToComplete);

		// 채굴 진행바를 채우기 위한 변수의 값을 점점 올림
		MiningProgressValue += GetWorld()->GetDeltaSeconds();
		
		// 채굴 완료, 인벤토리에 채굴한 아이템 삽입
		if (MiningProgressValue >= MiningTimeToComplete)
		{
			MiningProgressValue = 0.f;
			MiningInterationWidget->MiningCompleteText->SetVisibility(ESlateVisibility::Visible);
			MiningInterationWidget->ShowMiningCompleteMessage(CurrentTargetTile->GetResourceType());
			// add(CurrentTargetTile->GetResourceType());
		}
	}

	// 채굴 몽타주가 재생되고 있지 않다면, 채굴 몽타주 재생
	if(!bIsMiningAnimationPlaying)
	{
		PlayMiningAnimation();
	}
}

void APlayerCharacter::StopMining()
{
	// 플레이어가 채굴 중이라면
	if (GetIsMining())
	{
		SetIsMining(false);

		ShowPickaxe(false);
	}

	// 채굴 정지
	{
		if (MiningInterationWidget)
		{
			MiningInterationWidget->SetVisibility(ESlateVisibility::Hidden);
		}

		// 채굴 진행바와 우클릭 누적 시간을 0으로
		MiningProgressValue = 0.0f;

		MiningHoldTime = 0.0f;
	}

	// 채굴 몽타주가 재생되고 있다면 몽타주를 멈춤.
	if (bIsMiningAnimationPlaying)
	{
		StopMiningAnimation();
	}
}

void APlayerCharacter::SetCanMine(bool CanMine)
{
	bCanMine = CanMine;
}

void APlayerCharacter::SetIsMining(bool IsMining)
{
	bIsMining = IsMining;
}

void APlayerCharacter::SetCurrentTargetTile(AResourceTile* InResourceTile)
{
	CurrentTargetTile = InResourceTile;
}

void APlayerCharacter::ShowPickaxe(bool bVisible)
{
	if (PickaxeMesh)
	{
		PickaxeMesh->SetVisibility(bVisible);
	}
}

void APlayerCharacter::PlayMiningAnimation()
{
	if (!MiningMontage || !GetMesh() || !bIsMining) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(MiningMontage))
	{
		AnimInstance->Montage_Play(MiningMontage);
		// 채굴 몽타주 재생 중임을 나타냄
		bIsMiningAnimationPlaying = true;
	}
}

void APlayerCharacter::StopMiningAnimation()
{
	if (!MiningMontage || !GetMesh() || bIsMining) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->Montage_IsPlaying(MiningMontage))
	{
		AnimInstance->Montage_Stop(0.0f, MiningMontage); // 부드럽게 끊기
	}
}

void APlayerCharacter::OnMiningMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 채굴 몽타주가 끝났음 (재생 완료)
	bIsMiningAnimationPlaying = false;
}