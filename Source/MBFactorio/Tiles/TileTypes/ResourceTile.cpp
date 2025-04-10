#include "ResourceTile.h"
#include "Components/BoxComponent.h"
#include "Character/PlayerCharacter.h"
#include "Tools/WJMController.h"

AResourceTile::AResourceTile()
{
	PrimaryActorTick.bCanEverTick = true;
    
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

    // 충돌을 위한 OverlapBox의 크기를 Mesh와 같게 해줌
    OverlapBox->SetBoxExtent(TileMesh->Bounds.BoxExtent);

    OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &AResourceTile::OnBoxBeginOverlap);
    OverlapBox->OnComponentEndOverlap.AddDynamic(this, &AResourceTile::OnBoxEndOverlap);
}

void AResourceTile::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        Player->SetCurrentTargetTile(this); // 오버랩된 캐릭터에게 자신을 넘김

        bIsPlayerNear = true;

        if (AController* Controller = Player->GetController())
        {
            if (AWJMController* PC = Cast<AWJMController>(Controller))
            {
                PC->SetDetectedStope(this);
                PC->SetPlayerNearStope(true);
            }
        }
    }
}

void AResourceTile::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        Player->SetCurrentTargetTile(nullptr);

        bIsPlayerNear = false;

        if (AController* Controller = Player->GetController())
        {
            if (AWJMController* PC = Cast<AWJMController>(Controller))
            {
                PC->SetDetectedStope(nullptr);
                PC->SetPlayerNearStope(false);
            }
        }

        // 한 틱 뒤에 다시 감지 시도
        //FTimerHandle DummyHandle;
        //Player->GetWorldTimerManager().SetTimerForNextTick([Player]()
        //    {
        //        Player->TryReDetectStope();
        //    });
    }
}

void AResourceTile::SetResourceType(EResourceType InType)
{
	ResourceType = InType;
}