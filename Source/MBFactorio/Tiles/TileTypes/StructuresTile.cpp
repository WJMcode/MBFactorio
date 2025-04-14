#include "Tiles/TileTypes/StructuresTile.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "Tools/MBFController.h"

AStructuresTile::AStructuresTile()
{
	// OverlapBox 세팅
	{
		OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
		OverlapBox->SetupAttachment(RootComponent);

		OverlapBox->SetGenerateOverlapEvents(true);
		OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		OverlapBox->SetCollisionObjectType(ECC_WorldDynamic);

		OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
		OverlapBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
		OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	}

	// CollisionBox 세팅
	{
		CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
		CollisionBox->SetupAttachment(RootComponent);

		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); // 충돌 감지 및 물리 활성화
		CollisionBox->SetCollisionObjectType(ECC_WorldDynamic); // 월드 동적 오브젝트로 설정

		// 충돌 응답을 설정합니다.
		CollisionBox->SetCollisionResponseToAllChannels(ECR_Ignore);  // 기본적으로 모든 충돌 무시
		CollisionBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);  // 캐릭터와 충돌을 막습니다
	}
}

void AStructuresTile::BeginPlay()
{
	Super::BeginPlay();
		
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AStructuresTile::OnBoxBeginOverlap);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AStructuresTile::OnBoxEndOverlap);

	// CollisionBox의 크기 설정.
	// Z축은 캐릭터의 높이만큼 설정해서 캐릭터가 지나갈 수 없도록 함
	CollisionBox->SetBoxExtent(FVector(18.f, 14.f, 180.f));
}

void AStructuresTile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("AStructuresTile::OnBoxBeginOverlap"));
	
	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (AController* Controller = Player->GetController())
		{
			if (AMBFController* PC = Cast<AMBFController>(Controller))
			{
				// 구조물 입장에서 봤을 때, 플레이어와 오버랩됨
				bIsPlayerNear = true;

				// 플레이어의 컨트롤러에 감지된 구조물을 세팅
				PC->SetDetectedStructures(this);

				// 플레이어 입장에서 봤을 때에도 구조물과 오버랩되었으므로 true를 넘김
				PC->SetPlayerNearObject(true);

				// 오버랩된 플레이어에게 자신(광물)을 넘김
				// @TODO: 구조물에 맞게 수정해야 함
				// Player->GetMiningComponent()->SetCurrentTargetTile(this);
			}
		}
	}
}

void AStructuresTile::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("AStructuresTile::OnBoxEndOverlap"));

	if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
	{
		if (AController* Controller = Player->GetController())
		{
			if (AMBFController* PC = Cast<AMBFController>(Controller))
			{
				// 구조물 입장에서 봤을 때, 플레이어와 오버랩 해제됨
				bIsPlayerNear = false;

				/* 구조물과 플레이어 사이의 오버랩이 해제되면,
					중복으로 오버랩되었던 다른 구조물을 찾음       */
				AStructuresTile* FoundStructures = Cast<AStructuresTile>(PC->FindOverlappingTile<AStructuresTile>());

				/* 오버랩 해제된 구조물 외의 다른 구조물이 오버랩된 상태라면
				   그 구조물을 DetectedStructures로 설정함                       */
				if (FoundStructures)
				{
					PC->SetDetectedStructures(FoundStructures);
					// 플레이어 입장에서는 광물과 오버랩된 상태이므로 true를 넘김
					PC->SetPlayerNearObject(true);					

					// 감지된 다른 광물을 플레이어에게 넘김  
					// @TODO: 구조물에 맞게 수정해야 함
					//Player->GetMiningComponent()->SetCurrentTargetTile(FoundStope);
				}
				/*  구조물과 플레이어 사이의 오버랩이 해제된 상태에서
					 다른 중복 오버랩된 구조물이 없다면 SetDetectedStructures를 nullptr로 세팅   */
				else
				{
					PC->SetDetectedStructures(nullptr);
					// 플레이어 입장에서도 구조물과 오버랩 해제된 상태이므로 false를 넘김
					PC->SetPlayerNearObject(false);

					// 아무 구조물도 감지되지 않았으므로 nullptr을 플레이어에게 넘김
					// @TODO: 구조물에 맞게 수정해야 함
					// Player->GetMiningComponent()->SetCurrentTargetTile(nullptr);
				}
			}
		}
	}
}

void AStructuresTile::SetStructuresType(EStructuresType InType)
{
	StructuresType = InType;
}

void AStructuresTile::SetStructuresTileScale(EStructuresType InType, float TileSize)
{
	// 화로는 세로가 두 배로 긴 형태이므로 X : Y = 1 : 2의 비율로 크기를 조정합니다.
	if (EStructuresType::Furnace == InType)
	{
		float MeshSize = GetMeshBaseSize();
		float Scale = TileSize / MeshSize;
		SetActorScale3D(FVector(Scale, Scale * 2.f, 1.f)); // 타일은 평면이기 때문에 Z 스케일은 그대로 두기
	}
}
