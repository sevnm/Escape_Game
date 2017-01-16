// Fill out your copyright notice in the Description page of Project Settings.

#include "Escape.h"
#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


void UGrabber::FindPhysicsHandleComponent()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	// Look for attached Physics Handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("Physics Handle found"), *GetOwner()->GetName())
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing physics handle component"), *GetOwner()->GetName())
	}
}

void UGrabber::SetupInputComponent()
{
	// Look for attached Input Component (Appears at runtime)
	InputComp = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComp)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input Component Found"), *GetOwner()->GetName())
			InputComp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComp->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s missing input component"), *GetOwner()->GetName())
	}
}

void UGrabber::GetPlayerViewPoint(FVector &PlayerLocation, FRotator &PlayerRotation)
{
	// Get player viewpoint
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerLocation,
		OUT PlayerRotation
	);
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	FindPhysicsHandleComponent();
	SetupInputComponent();

}




void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Pressed"))

	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	auto ActorHit = HitResult.GetActor();
	
	
	if (ActorHit) {
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true // Allows rotation
		);
	}
		
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber Released"))
	PhysicsHandle->ReleaseComponent();
}

// Debug line for reach
void UGrabber::DrawReachLine(FVector PlayerViewPointLocation, FVector LineTraceEnd)
{
	DrawDebugLine(
	GetWorld(),
	PlayerViewPointLocation,
	LineTraceEnd,
	FColor(50, 255, 0),
	false,
	0.f,
	0.f,
	10.f
	);
}


const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;

	GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);

	/*UE_LOG(LogTemp,Warning,TEXT("Location: %s Position: %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString())*/
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	// DrawReachLine(PlayerViewPointLocation, LineTraceEnd);

	FHitResult Hit;
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	//Ray Cast and stores object type in Hit
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);
	AActor* HitObject = Hit.GetActor();
	if (HitObject)
	{
		UE_LOG(LogTemp, Warning, TEXT("Lets grab that %s"), *(HitObject->GetName()))
			
	}
	return Hit;
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	GetPlayerViewPoint(PlayerViewPointLocation, PlayerViewPointRotation);
	LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
	
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}
}

