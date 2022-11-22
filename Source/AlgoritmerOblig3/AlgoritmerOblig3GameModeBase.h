// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SphereActor.h"
#include "AlgoritmerOblig3GameModeBase.generated.h"


/**
 * 
 */
UCLASS()
class ALGORITMEROBLIG3_API AAlgoritmerOblig3GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void SpawnSpheres(int SpawnAmount);
	void DeleteSpheres();
	FVector GetRandomSpawnNumbers();
	bool bHasNodesSpawned = false;
	bool bHasReachedEnd = false;

	void DrawDebugLines();
	bool CheckConnection(ASphereActor* RDM);
	void RunAlgorithm(bool bAStarIsRunning);
	void ResetPath();
	void InformedSearch();
	int DistanceToEnd(ASphereActor* node);

	void ChangeMaterials(TArray<ASphereActor*> &mArray);

	ASphereActor* EndNode;
	ASphereActor* StartNode;
	TArray<ASphereActor*> SearchedNodes;
	TArray<ASphereActor*> PriorityQueue;
	TArray<ASphereActor*> VisitedNodes;

private:
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ASphereActor> SphereActor_BP;

	TArray<ASphereActor*> AllNodesList;
	TArray<ASphereActor*> ChunkArray;
	bool bHasConstructedChunk = false;
};