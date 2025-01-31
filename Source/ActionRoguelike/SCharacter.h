// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class USAttributeComponent;
class USpringArmComponent;
class UCameraComponent;
class USInteractionComponent;
class UAnimMontage;

UCLASS()
class ACTIONROGUELIKE_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ProjectileClass;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<USpringArmComponent> SpringArgComponent;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USInteractionComponent> InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<USAttributeComponent> AttributeComponent;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<UAnimMontage> AttackAnimMontage;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	float PrimaryAttackDelay;

	UPROPERTY(EditDefaultsOnly, Category="Visuals")
	TSubclassOf<UUserWidget> CrosshairWidgetAsset;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward(float value);

	void MoveRight(float value);

	void PrimaryAttack_Execution();
	
	void PrimaryAttack();

	void PrimaryInteract();

private:

	FTimerHandle TimerHandle_PrimaryAttack;

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
