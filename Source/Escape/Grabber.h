// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	FVector LineTraceEnd;


private:
	// How far Players reach is
	float Reach = 100.f;
	
	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComp = nullptr;

	//Raycast and Grab Actor in Reach
	void Grab();
	void Release();

	void FindPhysicsHandleComponent();
	void SetupInputComponent();
	// Used to cast Rays



	void GetPlayerViewPoint(FVector &PlayerLocation, FRotator &PlayerRotation); \
		// Return hit for first physics body in reach
	const FHitResult GetFirstPhysicsBodyInReach();
	void DrawReachLine(FVector PlayerViewPointLocation, FVector LineTraceEnd);
};
