// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_RangedAttack.h"

#include "AIController.h"
#include "ActionRoguelike/SCharacter.h"
#include "ActionRoguelike/SMagicProjectile.h"
#include "AI/SAICharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

EBTNodeResult::Type USBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	if(ensure(AIController))
	{
		const ASAICharacter* AIPawn = Cast<ASAICharacter>(AIController->GetPawn());
		if(AIPawn == nullptr)
		{
			return EBTNodeResult::Failed;	
		}
		
		const ASCharacter* TargetActor = Cast<ASCharacter>(OwnerComp.GetBlackboardComponent()->GetValueAsObject("TargetActor"));
		if(TargetActor == nullptr)
		{
			return EBTNodeResult::Failed;
		}

		const FVector MuzzleLocation = AIPawn->GetMesh()->GetSocketLocation("Muzzle_01");
		const FVector Direction = TargetActor->GetActorLocation() - MuzzleLocation;
		const FRotator MuzzleRotation = Direction.Rotation();

		FActorSpawnParameters Params;
		Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		const AActor* SpawnedProjectile = GetWorld()->SpawnActor<AActor>(AIPawn->ProjectileActor, MuzzleLocation, MuzzleRotation, Params);
		
		return SpawnedProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}

	return EBTNodeResult::Failed;
}
