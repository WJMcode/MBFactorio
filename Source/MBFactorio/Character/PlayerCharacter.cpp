#include "Character/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// 캐릭터 회전 관련 설정
	{
		bUseControllerRotationYaw = false; // 컨트롤러 Yaw 회전 비활성화
		GetCharacterMovement()->bOrientRotationToMovement = true; // 이동 방향으로 자동 회전
	}

	GetCharacterMovement()->RotationRate = FRotator(0.f, 360.f, 0.f); // 회전 속도 증가
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
	EnhancedInput->BindAction(OpenInventoryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OpenInventory);
}

void APlayerCharacter::MoveCharacter(const FInputActionValue& Value)
{
	FVector2D InputVector = Value.Get<FVector2D>();

	if (Controller)
	{
		FVector MoveDirection = FVector(InputVector.Y, InputVector.X, 0.f);
		if (!MoveDirection.IsNearlyZero()) // 입력이 있을 때만 처리
		{
			AddMovementInput(MoveDirection);

			// 이동 방향을 바라보도록 회전, 빠른 방향 전환
			FRotator NewRotation = MoveDirection.Rotation();
			SetActorRotation(NewRotation);
		}

	}
}

void APlayerCharacter::OpenInventory(const FInputActionValue& Value)
{

}
