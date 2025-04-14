// 2025 04 13 : ALYJController로 캐스팅 및 주석처리 (UResourceTile::OnBoxBeginOverlap, OnBoxEndOverlap) (이윤정)
// 2025 04 13 : 헤더 파일 Include (이윤정)
// 2025 04 15 : 통합 컨트롤러 사용하도록 수정

#include "ResourceTile.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "Tools/MBFController.h"
#include "Component/Mining/MiningComponent.h"

AResourceTile::AResourceTile()
{
	PrimaryActorTick.bCanEverTick = false;
    
    OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
    OverlapBox->SetupAttachment(RootComponent);
    
    OverlapBox->SetGenerateOverlapEvents(true);
    OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    OverlapBox->SetCollisionObjectType(ECC_WorldDynamic);
        
    OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
    OverlapBox->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
    OverlapBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void AResourceTile::BeginPlay()
{
    Super::BeginPlay();

    // 오버랩을 위한 OverlapBox의 크기를 Mesh 크기와 같게 설정
    FVector MeshBoxExtent = TileMesh->Bounds.BoxExtent;
    MeshBoxExtent.Z = 10.f; // 높이는 적당히
    OverlapBox->SetBoxExtent(MeshBoxExtent);

    OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AResourceTile::OnBoxBeginOverlap);
    OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AResourceTile::OnBoxEndOverlap);
}

void AResourceTile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        if (AController* Controller = Player->GetController())
        {            
            if (AMBFController* PC = Cast<AMBFController>(Controller))
            {
                // 광물 입장에서 봤을 때, 플레이어와 오버랩됨
                bIsPlayerNear = true;

                // 플레이어의 컨트롤러에 감지된 광물을 세팅
                PC->SetDetectedStope(this);

                // 플레이어 입장에서 봤을 때에도 광물과 오버랩되었으므로 true를 넘김
                PC->SetPlayerNearObject(true);

                // 오버랩된 플레이어에게 자신(광물)을 넘김
                Player->GetMiningComponent()->SetCurrentTargetTile(this);
            }
        }
    }
}

void AResourceTile::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        if (AController* Controller = Player->GetController())
        {
            if (AMBFController* PC = Cast<AMBFController>(Controller))
            {
                // 광물 입장에서 봤을 때, 플레이어와 오버랩 해제됨
                bIsPlayerNear = false;

                /* 광물과 플레이어 사이의 오버랩이 해제되면, 
                    중복으로 오버랩되었던 다른 광물을 찾음       */
                AResourceTile* FoundStope = Cast<AResourceTile>(PC->FindOverlappingTile<AResourceTile>());
                
                 /* 오버랩 해제된 광물 외의 다른 광물이 오버랩된 상태라면
                    그 광물을 DetectedStope으로 설정함                       */
                if (FoundStope)
                {
                    PC->SetDetectedStope(FoundStope);
                    // 플레이어 입장에서는 광물과 오버랩된 상태이므로 true를 넘김
                    PC->SetPlayerNearObject(true);
                    
                    // 감지된 다른 광물을 플레이어에게 넘김  
                    Player->GetMiningComponent()->SetCurrentTargetTile(FoundStope);
                }
                /*  광물과 플레이어 사이의 오버랩이 해제된 상태에서 
                     다른 중복 오버랩된 광물이 없다면 DetectedStope을 nullptr로 세팅   */
                else
                {
                    PC->SetDetectedStope(nullptr);
                    // 플레이어 입장에서도 광물과 오버랩 해제된 상태이므로 false를 넘김
                    PC->SetPlayerNearObject(false);

                    // 아무 광물도 감지되지 않았으므로 nullptr을 플레이어에게 넘김
                    Player->GetMiningComponent()->SetCurrentTargetTile(nullptr);
                }
            }
        }
    }
}

void AResourceTile::SetResourceType(EResourceType InType)
{
	ResourceType = InType;
}