#include "MBFStope.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
//#include "Character/LYJCharacter.h"
#include "Character/PlayerCharacter.h"
#include "Tools/MBFController.h" 

// 2025.04.10 GameHUD 테스트 위해 PlayerCharacter 활용 중

AMBFStope::AMBFStope()
{
    PrimaryActorTick.bCanEverTick = true;

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    RootComponent = Box;

    Box->SetGenerateOverlapEvents(true);
    Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Box->SetCollisionObjectType(ECC_WorldDynamic);

    Box->SetCollisionResponseToAllChannels(ECR_Ignore);
    Box->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block); 
    Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

    StopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StopeMesh->SetupAttachment(RootComponent);
    StopeMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 1.f));
    StopeMesh->SetRelativeLocation(FVector(0.f, 0.f, -30.f));

    static ConstructorHelpers::FObjectFinder<UStaticMesh> StopeAsset(TEXT("/Script/Engine.StaticMesh'/Engine/VREditor/WorldMovementGrid/PlaneMesh.PlaneMesh'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> StopeMatAsset(TEXT("/Script/Engine.Material'/Game/wjm_test/Tiles/OreTile/Stone/StoneOre05_Mat.StoneOre05_Mat'"));
    if (StopeAsset.Succeeded())
    {
        StopeMesh->SetStaticMesh(StopeAsset.Object);
        StopeMesh->SetMaterial(0, StopeMatAsset.Object);
    }
}

void AMBFStope::BeginPlay()
{
    Super::BeginPlay();

    Box->OnComponentBeginOverlap.AddDynamic(this, &AMBFStope::OnBoxBeginOverlap);
    Box->OnComponentEndOverlap.AddDynamic(this, &AMBFStope::OnBoxEndOverlap);
}

void AMBFStope::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        bIsPlayerNear = true;

        if (AController* Controller = Player->GetController())
        {
            if (AMBFController* PC = Cast<AMBFController>(Controller))
            {
                PC->SetPlayerNearObject(true);
            }
        }
    }

    /*if (ALYJCharacter* Player = Cast<ALYJCharacter>(OtherActor))
    {
        bIsPlayerNear = true;

        if (AController* Controller = Player->GetController())
        {
            if (AMBFController* PC = Cast<AMBFController>(Controller))
            {
                PC->SetPlayerNearStope(true);
            }
        }
    }*/
}

void AMBFStope::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        bIsPlayerNear = false;

        if (AController* Controller = Player->GetController())
        {
            if (AMBFController* PC = Cast<AMBFController>(Controller))
            {
                PC->SetPlayerNearObject(false);
            }
        }
    }

    /*if (ALYJCharacter* Player = Cast<ALYJCharacter>(OtherActor))
    {
        bIsPlayerNear = false;

        if (AController* Controller = Player->GetController())
        {
            if (AMBFController* PC = Cast<AMBFController>(Controller))
            {
                PC->SetPlayerNearStope(false);
            }
        }
    }*/
}

void AMBFStope::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
