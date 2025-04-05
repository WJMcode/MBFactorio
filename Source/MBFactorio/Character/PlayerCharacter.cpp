#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터가 이동 방향으로 회전하도록 합니다.
	{
		bUseControllerRotationYaw = false; // 컨트롤러 Yaw 회전 비활성화
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
			}
		}
	}
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input용 컴포넌트로 캐스팅
	UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	// InputAction을 C++ 함수에 바인딩
	EnhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::MoveCharacter);
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& Value)
{
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

