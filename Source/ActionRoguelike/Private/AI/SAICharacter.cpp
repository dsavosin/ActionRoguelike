// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SAICharacter.h"

#include "BrainComponent.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "AI/SAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/PawnSensingComponent.h"

// Sets default values
ASAICharacter::ASAICharacter()
{
	AttributeComponent = CreateDefaultSubobject<USAttributeComponent>("AttributeComponent");
	PawnSensingComponent = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensing");

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
void ASAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void ASAICharacter::OnHealthChanged(AActor* InstigatorActor, USAttributeComponent* OwningComp, float NewHealth, float Delta)
{
	if(NewHealth <= 0.0f)
	{
		// Stop BT
		if(TObjectPtr<ASAIController> AIController = Cast<ASAIController>(GetController()))
		{
			AIController->GetBrainComponent()->StopLogic("Killed");
		}
		
		// Ragdoll
		GetMesh()->SetAllBodiesSimulatePhysics(true);
		GetMesh()->SetCollisionProfileName("Ragdoll");
		
		// Set Lifespan
		SetLifeSpan(10.0f);
	}
}

void ASAICharacter::OnPawnSeen(APawn* Pawn)
{
	ASAIController* AIController = Cast<ASAIController>(GetController());
	if(AIController)
	{
		AIController->GetBlackboardComponent()->SetValueAsObject("TargetActor", Pawn);

		DrawDebugString(GetWorld(), GetActorLocation(), "PLAYER SPOTTED", nullptr, FColor::White, 4.0f, true);
	}
}

void ASAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	PawnSensingComponent->OnSeePawn.AddDynamic(this, &ASAICharacter::OnPawnSeen);
	AttributeComponent->OnHealthChanged.AddDynamic(this, &ASAICharacter::OnHealthChanged);
}


