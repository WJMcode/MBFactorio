#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Tiles/TileTypes/ResourceTile.h"

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

//void APlayerCharacter::TryReDetectStope()
//{
//	TArray<AActor*> OverlappingActors;
//	GetOverlappingActors(OverlappingActors, AResourceTile::StaticClass());
//
//	AResourceTile* NewTarget = nullptr;
//	for (AActor* Actor : OverlappingActors)
//	{
//		if (AResourceTile* Tile = Cast<AResourceTile>(Actor))
//		{
//			NewTarget = Tile;
//			break; // 하나만 기억하는 구조니까 하나만
//		}
//	}
//
//	SetCurrentTargetTile(NewTarget);
//}

void APlayerCharacter::TryStartMining()
{
	if (!CurrentTargetTile) return;

	// 우클릭을 쭉 누르면 MiningHoldTime가 점점 증가
	MiningHoldTime += GetWorld()->GetDeltaSeconds();

	// MiningHoldTime가 MinHoldTimeToPlayAnim보다 크거나 같으면 채굴 시작
	if (MiningHoldTime >= MinHoldTimeToPlayAnim)
	{
		StartMining();
	}
}

void APlayerCharacter::StartMining()
{
	SetIsMining(true);

	// 채굴 중인 상태라면 곡괭이를 듦
	// 그리고 채굴 몽타주가 재생되고 있지 않다면 채굴 몽타주 재생
	if(bIsMining)
	{
		ShowPickaxe(true);
		if (!bIsMiningAnimationPlaying)
		{
			PlayMiningAnimation();
		}
	}
}

void APlayerCharacter::StopMining()
{
	SetIsMining(false);
	
	// 채굴 중인 상태가 아니라면 곡괭이를 숨기고, 몽타주를 멈춤.
	// 그리고 채굴 진행바와 우클릭을 유지한 누적 시간을 0으로
	if(!bIsMining)
	{
		ShowPickaxe(false);
		StopMiningAnimation();
		MiningProgress = 0.0f;

		MiningHoldTime = 0.0f;
	}
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