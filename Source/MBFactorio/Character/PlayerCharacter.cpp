#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Component/Mining/MiningComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터가 이동 방향으로 회전하도록 합니다.
	{
		bUseControllerRotationYaw = false; // 컨트롤러 Yaw 회전 비활성화
	}

	MiningComponent = CreateDefaultSubobject<UMiningComponent>(TEXT("MiningComponent"));

	PickaxeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickaxeMesh"));
	PickaxeMesh->SetupAttachment(GetMesh(), TEXT("PickaxeSocket")); // 소켓 이름
	PickaxeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ShowPickaxe(false); // 곡괭이가 기본적으로 안 보이게 설정
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
		EnhancedInput->BindAction(MiningAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnMiningTriggered);
		EnhancedInput->BindAction(MiningAction, ETriggerEvent::Completed, this, &APlayerCharacter::OnMiningReleased);

		// 아이템 드랍 관련 액션
		EnhancedInput->BindAction(DropItemAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OnDropItemTriggered);
		EnhancedInput->BindAction(DropItemAction, ETriggerEvent::Started, this, &APlayerCharacter::OnDropItemStarted);
		EnhancedInput->BindAction(DropItemAction, ETriggerEvent::Completed, this, &APlayerCharacter::OnDropItemReleased);

		// 캐릭터 움직임 관련 액션
		EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveCharacter);
	}
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& Value)
{
	// 채굴 중에 움직이면 채굴을 멈춤
	if (MiningComponent->IsMining())
	{
		MiningComponent->StopMining();
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

void APlayerCharacter::ShowPickaxe(bool bVisible)
{
	if (PickaxeMesh)
	{
		PickaxeMesh->SetVisibility(bVisible);
	}
}

void APlayerCharacter::OnMiningTriggered()
{
	MiningComponent->TryStartMining();
}

void APlayerCharacter::OnMiningReleased()
{
	MiningComponent->StopMining();
}

void APlayerCharacter::PlayMiningAnimation()
{
	if (!MiningMontage || !GetMesh() || !MiningComponent->IsMining()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && !AnimInstance->Montage_IsPlaying(MiningMontage))
	{
		AnimInstance->Montage_Play(MiningMontage);
		// 채굴 몽타주 재생 중임을 나타냄
		MiningComponent->SetMiningAnimationPlaying(true);
	}
}

void APlayerCharacter::StopMiningAnimation()
{
	if (!MiningMontage || !GetMesh() || MiningComponent->IsMining()) return;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AnimInstance->Montage_IsPlaying(MiningMontage))
	{
		AnimInstance->Montage_Stop(0.0f, MiningMontage); // 부드럽게 끊기
	}
}

void APlayerCharacter::OnMiningMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// 채굴 몽타주가 끝났음 (재생 완료)
	MiningComponent->SetMiningAnimationPlaying(false);
}

void APlayerCharacter::OnDropItemTriggered()
{
	UE_LOG(LogTemp, Warning, TEXT("APlayerCharacter::OnDropItemTriggered()"));

}

void APlayerCharacter::OnDropItemStarted()
{
	UE_LOG(LogTemp, Warning, TEXT("AStructuresTile::OnDropItemStarted"));

}

void APlayerCharacter::OnDropItemReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("AStructuresTile::OnDropItemReleased"));

}
