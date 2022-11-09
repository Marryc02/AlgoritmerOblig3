// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASphereActor::ASphereActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SphereMesh"));
	SetRootComponent(SphereMesh);
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetupAttachment(GetRootComponent());

	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASphereActor::OnOverlap);

}

// Called when the game starts or when spawned
void ASphereActor::BeginPlay()
{
	Super::BeginPlay();
	float x = FMath::RandRange(1,3);
	SphereMesh->SetWorldScale3D(FVector(1, 1, 1) * x);

	SphereMeshScale = SphereMesh->GetComponentScale();

	CollisionSphere->SetWorldScale3D(SphereMeshScale * 10);
	Position = GetActorLocation();
}

void ASphereActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComponent, int32 OtherbodyIndex, bool bFromSweep, const FHitResult& SweepResult) {

	if (OtherActor->IsA(ASphereActor::StaticClass())) {
		ASphereActor* CollidedNode = Cast<ASphereActor>(OtherActor);
		if (ConnectedNodesList.Find(CollidedNode) == INDEX_NONE) {
			if (CollidedNode->ConnectedNodesList.Num() != 0) {
				for (int i = 0; i < CollidedNode->ConnectedNodesList.Num(); i++) {
					if (CollidedNode->ConnectedNodesList.Find(this) != INDEX_NONE) {
						this->ConnectedNodesList.Add(CollidedNode);
						//this->CollisionSphere->DestroyComponent();
						return;
					}
					else {
						CollidedNode->ConnectedNodesList.Add(this);
						this->ConnectedNodesList.Add(CollidedNode);
						//this->CollisionSphere->DestroyComponent();
						return;
					}
				}
			}
			else {
				this->ConnectedNodesList.Add(CollidedNode);
				CollidedNode->ConnectedNodesList.Add(this);
				//this->CollisionSphere->DestroyComponent();
				return;
			}
		}
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
		i += 0.2;

		if (this->ConnectedNodesList.Num() > 0) {
			UE_LOG(LogTemp, Warning, TEXT("DSAHODHODAHPOIUDAHPOD"));
			break;
		}

		/*if (i >= 10.f) {
			break;
		}*/
	}
}

void ASphereActor::ConnectNodes() {

}
