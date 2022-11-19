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

	bool bIsStartNode = false;
	bool bIsEndNode = false;
	bool bIsPathNode = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterial* StartMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterial* EndMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterial* PathMat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UMaterial* ShortMat;


	FVector SphereMeshScale;

	UFUNCTION()
		void IncreaseCollisionSphereSize();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ASphereActor*> ConnectedNodesList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TArray<ASphereActor*> PathToGetTo;

	UPROPERTY(EditAnywhere)
		FVector Position;
	UPROPERTY()
		int Cost;
	UPROPERTY()
		int ScaleCost;
	UPROPERTY()
		FVector Distance;
	UPROPERTY()
		int CalculatedDistance;

	UFUNCTION()
		int CalculateDistance(FVector dis);


	UFUNCTION()
		void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent,
			int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
