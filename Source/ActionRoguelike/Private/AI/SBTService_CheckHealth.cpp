// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/SBTService_CheckHealth.h"

#include "AIController.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

USBTService_CheckHealth::USBTService_CheckHealth()
{
	LowHealthThreshold = 20.0f;
	RestoredHealthThreshold = 100.0f;
}

void USBTService_CheckHealth::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* BlackboardComp = OwnerComp.GetBlackboardComponent();
	if(!ensure(BlackboardComp))
	{
		return;
	}
	
	const APawn* AIPawn = OwnerComp.GetAIOwner()->GetPawn();
	if(!IsValid(AIPawn))
	{
		return;
	}
	
	if(const USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(AIPawn->GetComponentByClass(USAttributeComponent::StaticClass())))
	{
		const float CurrentHealth = AttributeComp->GetHealth();
		if(CurrentHealth < LowHealthThreshold && AttributeComp->IsAlive())
		{
			BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, true);
		}

		if (CurrentHealth >= RestoredHealthThreshold)
		{
			BlackboardComp->SetValueAsBool(LowHealthKey.SelectedKeyName, false);
		}
	}
	
}
