// Fill out your copyright notice in the Description page of Project Settings.


#include "SInteractionComponent.h"

#include "SGameplayInterface.h"
#include "DSP/AudioDebuggingUtilities.h"
#include "Evaluation/IMovieSceneEvaluationHook.h"

// Sets default values for this component's properties
USInteractionComponent::USInteractionComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USInteractionComponent::PrimaryInteract()
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	
	FVector EyeLocation;
	FRotator EyeRotation;
	GetOwner()->GetActorEyesViewPoint(EyeLocation, EyeRotation);
	
	const FVector End = EyeLocation + (EyeRotation.Vector() * 1000.0f);
	
	//FHitResult Hit;
	//bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, EyeLocation, End, ObjectQueryParams);

	FCollisionShape Sphere;
	Sphere.SetSphere(30.0f);
		
	TArray<FHitResult> Hits;
	const bool bBlockingHit = GetWorld()->SweepMultiByObjectType(Hits, EyeLocation, End, FQuat::Identity, ObjectQueryParams, Sphere);
	FColor HitColor = FColor::Red;
	
	for (const FHitResult& Hit : Hits)
	{
		if(AActor* HitActor = Hit.GetActor())
		{
			if(HitActor->Implements<USGameplayInterface>())
			{
				const APawn* OwningPawn = Cast<APawn>(GetOwner());
				ISGameplayInterface::Execute_Interact(HitActor, OwningPawn);
				HitColor = FColor::Green;
				DrawDebugSphere(GetWorld(), Hit.ImpactPoint, 30.0f, 16, HitColor,false, 2.0f, 0.0f, 2.0f );
				break;
			}
		}
	}
	
	DrawDebugLine(GetWorld(), EyeLocation, End, HitColor, false, 2.0f, 0.0f, 2.0f);
}


// Called when the game starts
void USInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void USInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

