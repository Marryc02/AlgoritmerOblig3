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

	StartMat = CreateDefaultSubobject<UMaterial>(TEXT("StartMat"));
	EndMat = CreateDefaultSubobject<UMaterial>(TEXT("EndMat"));
	PathMat = CreateDefaultSubobject<UMaterial>(TEXT("PathMat"));


	CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &ASphereActor::OnOverlap);

}

// Called when the game starts or when spawned
void ASphereActor::BeginPlay()
{
	Super::BeginPlay();

	// Change the scale of the mesh and therefore also the collisionbox
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

		// First check if the node you just collided with is already in your ConnectedNodeList
		// If so we don't need to add it, otherwise we do
		if (ConnectedNodesList.Find(CollidedNode) == INDEX_NONE) {
			// Check now if the other node has any nodes in its list, if it does we need to see if
			// we are in that list
			if (CollidedNode->ConnectedNodesList.Num() != 0) {
				for (int i = 0; i < CollidedNode->ConnectedNodesList.Num(); i++) {
					if (CollidedNode->ConnectedNodesList.Find(this) != INDEX_NONE) {
						// If we are in that list, we add them to our list
						// we do not add ourselves to their list, as that would create a dupilicate
						this->ConnectedNodesList.Add(CollidedNode);
						return;
					}
					else {
						// If we are not in that list we will add ourselves to their list
						// and we will add them to ours
						CollidedNode->ConnectedNodesList.Add(this);
						this->ConnectedNodesList.Add(CollidedNode);
						return;
					}
				}
			}
			else {
				// If the other node has an empty list we put them in our list and
				// add ourselves to their list
				this->ConnectedNodesList.Add(CollidedNode);
				CollidedNode->ConnectedNodesList.Add(this);
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
		// It stops once it has another node in it's ConnectedNodesList
		// Now we ensure there are no stragglers that have no nodes in their list
		CollisionSphere->SetWorldScale3D(OldFVector * i);
		i += 0.2;

		if (this->ConnectedNodesList.Num() > 0) {
			break;
		}
	}
}