// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "SphereActor.generated.h"


UCLASS()
class ALGORITMEROBLIG3_API ASphereActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASphereActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SphereMesh")
		class UStaticMeshComponent* SphereMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "CollisionSphere")
		class USphereComponent* CollisionSphere = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NoSpawnSphere")
		class USphereComponent* NoSpawnSphere = nullptr;

	FVector SphereMeshScale;

	UFUNCTION()
		void IncreaseCollisionSphereSize();
	UFUNCTION()
		void ConnectNodes();

	TArray<ASphereActor*> ConnectedNodesList;

	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void Teleport();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
