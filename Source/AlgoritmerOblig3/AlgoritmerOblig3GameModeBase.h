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

private:
	UPROPERTY(EditAnywhere, Category = "Spawning")
		TSubclassOf<class ASphereActor> SphereActor_BP;
};