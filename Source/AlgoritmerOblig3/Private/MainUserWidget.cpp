// Fill out your copyright notice in the Description page of Project Settings.


#include "MainUserWidget.h"
#include "Components/TextBlock.h"
#include "Components/Slider.h"
#include "Components/Button.h"



bool UMainUserWidget::Initialize() {
	Super::Initialize();

	if (SpawnNodesButton) {
		SpawnNodesButton->OnClicked.AddDynamic(this, &UMainUserWidget::ClickSpawnNodes);
	}
	if (DeleteNodesButton) {
		DeleteNodesButton->OnClicked.AddDynamic(this, &UMainUserWidget::ClickDeleteNodes);
	}
	if (RunDjikstraButton) {
		RunDjikstraButton->OnClicked.AddDynamic(this, &UMainUserWidget::ClickDjikstraButton);
	}
	if (RunAStarButton) {
		RunAStarButton->OnClicked.AddDynamic(this, &UMainUserWidget::ClickAStarButton);
	}

	return true;
}

void UMainUserWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime) {
	Super::NativeTick(MyGeometry, InDeltaTime);

	NodeAmountText->SetText(FText::FromString(FString::FromInt(NodeSlider->GetValue())));
}

void UMainUserWidget::ClickSpawnNodes() {
	UE_LOG(LogTemp, Warning, TEXT("HIT SPAWN NODES BUTTON"));
	GameModePtr->SpawnSpheres(NodeSlider->GetValue());
}

void UMainUserWidget::ClickDeleteNodes() {
	UE_LOG(LogTemp, Warning, TEXT("HIT DELETE NODES BUTTON"));
	GameModePtr->DeleteSpheres();
}

void UMainUserWidget::ClickDjikstraButton() {
	UE_LOG(LogTemp, Warning, TEXT("HIT DJIKSTRA BUTTON"));
	GameModePtr->RunDjikstra();
}

void UMainUserWidget::ClickAStarButton() {
	UE_LOG(LogTemp, Warning, TEXT("HIT A STAR BUTTON"));
}
