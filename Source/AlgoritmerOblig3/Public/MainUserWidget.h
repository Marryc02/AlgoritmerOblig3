// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../AlgoritmerOblig3GameModeBase.h"
#include "MainUserWidget.generated.h"


/**
 * 
 */
UCLASS()
class ALGORITMEROBLIG3_API UMainUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	virtual bool Initialize();

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	AAlgoritmerOblig3GameModeBase* GameModePtr = Cast<AAlgoritmerOblig3GameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UButton* SpawnNodesButton;
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UButton* DeleteNodesButton;
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class USlider* NodeSlider;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UButton* RunDjikstraButton;
	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UButton* RunAStarButton;

	UPROPERTY(BlueprintReadWrite, Category = "UI", meta = (BindWidget))
		class UTextBlock* NodeAmountText;

	UFUNCTION()
	void ClickSpawnNodes();
	UFUNCTION()
	void ClickDeleteNodes();
	UFUNCTION()
	void ClickDjikstraButton();
	UFUNCTION()
	void ClickAStarButton();
};
