// Fill out your copyright notice in the Description page of Project Settings.


#include "SGameModeBase.h"

#include "EngineUtils.h"
#include "ActionRoguelike/SAttributeComponent.h"
#include "AI/SAICharacter.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ASGameModeBase::ASGameModeBase()
{
	SpawnTimerInterval = 2.0f;
	MaxBotsCount = 5;
}

void ASGameModeBase::StartPlay()
{
	Super::StartPlay();

	GetWorldTimerManager().SetTimer(TimerHandle_SpawnBots, this, &ASGameModeBase::SpawnBotTimerElapsed, SpawnTimerInterval, true);
}

// void ASGameModeBase::OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance,
// 	EEnvQueryStatus::Type QueryStatus)
// {
// 	if(QueryStatus != EEnvQueryStatus::Success)
// 	{
// 		UE_LOG(LogTemp, Warning, TEXT("Spawn Bot EQS Query Failed!"));
// 		return;
// 	}
//
// 	int32 NumOfAliveBots = 0;
// 	for (TObjectPtr<ASAICharacter> Bot: TActorRange<ASAICharacter>(GetWorld()))
// 	{
// 		TObjectPtr<USAttributeComponent> AIAttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
// 		if (ensure(AIAttributeComp) && AIAttributeComp->IsAlive())
// 		{
// 			++NumOfAliveBots;
// 		}
// 	}
//
// 	if(DifficultyCurve)
// 	{
// 		MaxBotsCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
// 	}
// 	
// 	if(NumOfAliveBots >= MaxBotsCount)
// 	{
// 		return;
// 	}
//
// 	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
// 	if(Locations.IsValidIndex(0))
// 	{
// 		GetWorld()->SpawnActor<AActor>(EnemyClass, Locations[0], FRotator::ZeroRotator);
// 	}
// }

void ASGameModeBase::OnQueryCompleted(TSharedPtr<FEnvQueryResult> Result)
{
	const TObjectPtr<FEnvQueryResult> QueryResult = Result.Get();
	if (!QueryResult->IsSuccessful())
	{
		UE_LOG(LogTemp, Warning, TEXT("Spawn bot EQS Query Failed!"));
		return;
	}

	int32 NumOfAliveBots = 0;
	for (const TObjectPtr<ASAICharacter> Bot: TActorRange<ASAICharacter>(GetWorld()))
	{
		TObjectPtr<USAttributeComponent> AIAttributeComp = Cast<USAttributeComponent>(Bot->GetComponentByClass(USAttributeComponent::StaticClass()));
		if (ensure(AIAttributeComp) && AIAttributeComp->IsAlive())
		{
			++NumOfAliveBots;
		}
	}

	if(DifficultyCurve)
	{
		MaxBotsCount = DifficultyCurve->GetFloatValue(GetWorld()->GetTimeSeconds());
	}
	
	if(NumOfAliveBots >= MaxBotsCount)
	{
		return;
	}
	
	// Retrieve all possible locations that passed the query
	TArray<FVector> Locations;
	QueryResult->GetAllAsLocations(Locations);
	if(Locations.IsValidIndex(0))
	{
		GetWorld()->SpawnActor<AActor>(EnemyClass, Locations[0], FRotator::ZeroRotator);
	}
}

void ASGameModeBase::SpawnBotTimerElapsed()
{
	// UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest5Pct, nullptr);
	// if(ensure(QueryInstance))
	// {
	// 	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ASGameModeBase::OnQueryCompleted);
	// }

	FEnvQueryRequest BotSpawnRequest(SpawnBotQuery, this);
	BotSpawnRequest.Execute(EEnvQueryRunMode::RandomBest5Pct, this, &ASGameModeBase::OnQueryCompleted);
}
