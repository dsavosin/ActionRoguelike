// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTTask_Heal.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type USBTTask_Heal::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UE_LOG(LogTemp, Warning , TEXT("USBTTask_Heal::ExecuteTask"))
	bNotifyTick = 1;
	bTickIntervals = 1;
	
	return EBTNodeResult::InProgress;
}

void USBTTask_Heal::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UE_LOG(LogTemp, Warning , TEXT("USBTTask_Heal::TickTask"))

	TObjectPtr<UBlackboardComponent> BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(!ensure(BlackboardComp))
	{
		UE_LOG(LogTemp, Error , TEXT("USBTTask_Heal::No Valid BB"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	
	const APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!IsValid(AIPawn))
	{
		UE_LOG(LogTemp, Error , TEXT("USBTTask_Heal::No Valid Pawn"))
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	
	if(USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass())))
	{
		if(AttributeComp->ApplyHealthChange(nullptr, 20.0f))
		{
			SetNextTickTime(NodeMemory, 0.5);
		}

		if(AttributeComp->GetHealth() >= 100.0f)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
