// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();
	Owner = GetOwner();
	if (!PressurePlate)
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing pressure plate"), *Owner->GetName())
	}
	ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

float UOpenDoor::MassOnPlate()
{
	float TotalMass = 0;
	TArray<AActor*> ActorsOnPlate;
	if (!PressurePlate)
	{
		return TotalMass;
	}
	PressurePlate->GetOverlappingActors(OUT ActorsOnPlate);
	
	for (const auto& Actor : ActorsOnPlate)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("Lets grab that");
	}

	UE_LOG(LogTemp, Warning, TEXT("Total Mass is %f"), TotalMass)
	return TotalMass;
}
void UOpenDoor::PollPlate()
{
	if (MassOnPlate() > TriggerMass)
	{
		//UE_LOG(LogTemp, Warning, TEXT("Open broadcasting"))
		OnOpen.Broadcast();
	}
	else
	{
		//UE_LOG(LogTemp, Warning, TEXT("Close broadcasting"))
		OnClose.Broadcast();
	}
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	// See if mass on plate is > TriggerMass
	PollPlate();
	
}
