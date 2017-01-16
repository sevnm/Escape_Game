// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "OpenDoor.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ESCAPE_API UOpenDoor : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UOpenDoor();

	// Called when the game starts
	virtual void BeginPlay() override;
	void OpenDoor();
	void CloseDoor();
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
	float DoorLastOpenTime;
	UPROPERTY(EditAnywhere)
	float OpenAngle = -90.0f;
	
	float CloseAngle = 0.0f;
	
	UPROPERTY(EditAnywhere)
		float DoorCloseDelay = 1.8f;
	
	UPROPERTY(EditAnywhere)
		ATriggerVolume* PressurePlate = nullptr;
	UPROPERTY(EditAnywhere)
		AActor* ActorThatOpens = nullptr;

	UPROPERTY(EditAnywhere)
		float TriggerMass = 100.f;

	void PollPlate();
	float MassOnPlate();
	//float TotalMass = 0;
	AActor* Owner = nullptr;
};
