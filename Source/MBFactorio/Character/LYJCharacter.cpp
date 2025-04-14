//// Fill out your copyright notice in the Description page of Project Settings.
//
//
//#include "Character/LYJCharacter.h"
//#include "Tools/LYJController.h"
//#include "EnhancedInputSubsystems.h"
//#include "EnhancedInputComponent.h"
//
//// Sets default values
//ALYJCharacter::ALYJCharacter()
//{
// 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
//	PrimaryActorTick.bCanEverTick = true;
//
//	// 캐릭터가 이동 방향으로 회전하도록 합니다.
//	{
//		bUseControllerRotationYaw = false; // 컨트롤러 Yaw 회전 비활성화
//	}
//}
//
//// Called when the game starts or when spawned
//void ALYJCharacter::BeginPlay()
//{
//	Super::BeginPlay();
//		
//	if (ALYJController* PC = Cast<ALYJController>(GetController()))
//	{
//		if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
//		{
//			if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
//				ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
//			{
//				// IMC 등록
//				Subsystem->AddMappingContext(MoveMappingContext, 0);
//
//				UE_LOG(LogTemp, Warning, TEXT("Character: MoveMappingContext 등록"));
//			}
//		}
//	}
//}
//
//// Called every frame
//void ALYJCharacter::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}
//
//// Called to bind functionality to input
//void ALYJCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
//{
//	Super::SetupPlayerInputComponent(PlayerInputComponent);
//
//	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
//	{
//		if (MoveAction)
//		{
//			Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALYJCharacter::MoveCharacter);
//		}
//	}
//}
//
//void ALYJCharacter::MoveCharacter(const FInputActionValue& Value)
//{
//	FVector2D InputVector = Value.Get<FVector2D>();
//
//	if (Controller && !InputVector.IsNearlyZero())
//	{
//		// 회전 기준은 그대로 카메라 방향 유지
//		const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
//		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
//		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
//
//		// 최종 이동 방향 계산
//		FVector MoveDir = (Forward * InputVector.Y + Right * InputVector.X).GetSafeNormal();
//
//		// 이동
//		AddMovementInput(MoveDir);
//
//		// 방향키를 누르면 캐릭터 즉시 회전
//		SetActorRotation(MoveDir.Rotation());
//	}
//}
//
