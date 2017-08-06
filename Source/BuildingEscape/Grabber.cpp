// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#ifndef OUT
#define OUT // Mark function parameters that are changed by the function
#endif

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PhysicsHandle) return;

	// If object grabbed, Move Object
	if (PhysicsHandle->GrabbedComponent) PhysicsHandle->SetTargetLocation(GetReachLineEnd());
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	FindPhysicsHandleComponent();

	SetupInputComponent();
}

// Look for attached Input Component
void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", EInputEvent::IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", EInputEvent::IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s needs a UInputComoonent Component!"), *GetOwner()->GetName());
	}
}

// Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle)
		UE_LOG(LogTemp, Error, TEXT("%s needs a UPhysicsHandle Component!"), *GetOwner()->GetName());
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabbed!"));

	// Line Trace to get PhyicsBody
	auto Hit = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = Hit.GetComponent();
	auto ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("You tried to grab the actor %s"), *ActorHit->GetName());
		if (PhysicsHandle)
			PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation());
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Released!"));
	PhysicsHandle->ReleaseComponent();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Line-Trace (Ray-cast) out to reach distance
	FHitResult Hit;
	auto QueryParams = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		GetReachLineStart(),
		GetReachLineEnd(),
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		QueryParams
	);

	return Hit;
}

FVector UGrabber::GetReachLineStart()
{
	// Get Player view point this tick
	FVector PlayerViewPointLocation; // LineTrace Start
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	return PlayerViewPointLocation;
}

FVector UGrabber::GetReachLineEnd()
{
	// Get Player view point this tick
	FVector PlayerViewPointLocation; // LineTrace Start
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	// Determine LineTrace End
	return PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * REACH);
}

