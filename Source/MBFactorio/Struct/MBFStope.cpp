#include "MBFStope.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Character/PlayerCharacter.h" 
#include "Kismet/GameplayStatics.h"

AMBFStope::AMBFStope()
{
    PrimaryActorTick.bCanEverTick = true;

    // 2025.04.06 메쉬, 머테리얼 설정 및 에셋 추가
    StopeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    RootComponent = StopeMesh;
    static ConstructorHelpers::FObjectFinder<UStaticMesh> StopeAsset(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
    static ConstructorHelpers::FObjectFinder<UMaterial> StopeMatAsset(TEXT("/Script/Engine.Material'/Engine/EditorMaterials/GridMaterial.GridMaterial'"));
    if (StopeAsset.Succeeded())
    {
        StopeMesh->SetStaticMesh(StopeAsset.Object);
        StopeMesh->SetMaterial(0, StopeMatAsset.Object);
    }

    Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
    Box->SetupAttachment(RootComponent);

    Box->SetRelativeScale3D(FVector(3.f, 3.f, 1.f));
    Box->SetGenerateOverlapEvents(true);
    Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    Box->SetCollisionResponseToAllChannels(ECR_Ignore);
    Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
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
    }
}

void AMBFStope::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor))
    {
        bIsPlayerNear = false;
    }
}

void AMBFStope::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
}
