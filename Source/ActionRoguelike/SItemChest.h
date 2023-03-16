// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ASItemChest : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASItemChest();

	void Interact_Implementation(const APawn* InstigatorPawn) override;

protected:

	UPROPERTY(EditDefaultsOnly)
	float AngleLidOpened;

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ChestMesh;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UStaticMeshComponent> ChestLid;
};
