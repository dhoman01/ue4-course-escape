// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "Components/ActorComponent.h"
#include "DrawDebugHelpers.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPE_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	// Length Actor can reach
	UPROPERTY(EditAnywhere)
	float REACH = 100;

	UPhysicsHandleComponent* PhysicsHandle = nullptr;
	UInputComponent* InputComponent = nullptr;

	// Get the first Component in REACH
	FHitResult GetFirstPhysicsBodyInReach();

	FVector GetReachLineStart();

	FVector GetReachLineEnd();

	// Bind to input to grab
	void Grab();

	// Bind to input to release
	void Release();

	// Setup the assumed InputComponent
	void SetupInputComponent();

	// Find the assumed PhysicsHandleComponent
	void FindPhysicsHandleComponent();	
};
