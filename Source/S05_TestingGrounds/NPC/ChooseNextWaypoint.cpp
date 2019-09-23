// Fill out your copyright notice in the Description page of Project Settings.

#include "ChooseNextWaypoint.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "PatrolRoute.h"
#include "BehaviorTree/BlackboardComponent.h"


EBTNodeResult::Type UChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//get patrol route
	auto AIController = OwnerComp.GetAIOwner();
	auto PatrolComponent = AIController->GetPawn()->FindComponentByClass<UPatrolRoute>();
	if(!ensure(PatrolComponent)) { return EBTNodeResult::Failed; }
	
	auto PatrolPoints = PatrolComponent->GetPatrolPoints();
	if (PatrolPoints.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("A guard is missing patrol points"))
		return EBTNodeResult::Failed;
	}

	//set next waypoint
	auto BlackboardComponent = OwnerComp.GetBlackboardComponent();
	auto Index = BlackboardComponent->GetValueAsInt(IndexKey.SelectedKeyName);
	BlackboardComponent->SetValueAsObject(WaypointKey.SelectedKeyName, PatrolPoints[Index]);

	//cycle index
	int32 NewIndex = (Index + 1) % PatrolPoints.Num();
	BlackboardComponent->SetValueAsInt(IndexKey.SelectedKeyName, NewIndex);
	
	return EBTNodeResult::Succeeded;
}


