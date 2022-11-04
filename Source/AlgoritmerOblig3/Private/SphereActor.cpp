// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereActor.h"

// Sets default values
ASphereActor::ASphereActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SetRootComponent(SphereMesh);
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(SphereMesh);

	NoSpawnSphere = CreateDefaultSubobject<USphereComponent>(TEXT("NoSpawnSphere"));
	NoSpawnSphere->SetupAttachment(SphereMesh);

}

// Called when the game starts or when spawned
void ASphereActor::BeginPlay()
{
	Super::BeginPlay();
	float x = FMath::RandRange(0.5, 2);
	SphereMesh->SetWorldScale3D(FVector(1, 1, 1) * x);

	SphereMeshScale = SphereMesh->GetComponentScale();

	NoSpawnSphere->SetWorldScale3D(SphereMeshScale * 4);
	CollisionSphere->SetWorldScale3D(SphereMeshScale * 10);
	
}

void ASphereActor::Teleport() {
	int x = FMath::RandRange(-500, 500);
	int y = FMath::RandRange(-500, 500);
	int z = FMath::RandRange(-500, 500);

	this->SetActorLocation(FVector(x, y, z));
}

void ASphereActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA(ASphereActor::StaticClass())) {
		Teleport();
	}
}

// Called every frame
void ASphereActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASphereActor::IncreaseCollisionSphereSize() {
	float i = 1.f;
	while (true) {
		FVector OldFVector = CollisionSphere->GetComponentScale();
		
		// Increases the size of the collision box of the sphere until it collides with something
		// Currently it might not always work, once the collision is set up
		// we can properly implement it
		CollisionSphere->SetWorldScale3D(OldFVector * i);
		i += 0.1;

		if (i >= 2.f) {
			break;
		}
	}
}

void ASphereActor::ConnectNodes() {

}
