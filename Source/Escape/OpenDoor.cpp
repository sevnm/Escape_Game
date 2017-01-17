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
void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, CloseAngle, 0.f));
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

	//UE_LOG(LogTemp, Warning, TEXT("Total Mass is %f"), TotalMass)
	return TotalMass;
}
void UOpenDoor::PollPlate()
{
	if (MassOnPlate() > TriggerMass)
	{
		OpenDoor();
		DoorLastOpenTime = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if (GetWorld()->GetTimeSeconds() - DoorLastOpenTime > DoorCloseDelay)
		{
			CloseDoor();
		}
		
		//TotalMass = 0;
	}
}


// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
	// See if mass on plate is > TriggerMass
	PollPlate();
	
}
