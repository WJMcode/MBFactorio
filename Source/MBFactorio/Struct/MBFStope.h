// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MBFStope.generated.h"

UCLASS()
class MBFACTORIO_API AMBFStope : public AActor
{
    GENERATED_BODY()

public:
    AMBFStope();

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
    virtual void Tick(float DeltaTime) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
    class UBoxComponent* Box;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
    class UStaticMeshComponent* StaticMesh;

    UPROPERTY(BlueprintReadOnly, Category = "State")
    bool bIsPlayerNear = false;
};

