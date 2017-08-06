// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"

#ifndef OUT
#define OUT // Mark function parameters that are changed by the function
#endif

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

	if(!PressurePlate)
		UE_LOG(LogTemp, Error, TEXT("%s is missing Pressure Plate!"), *GetOwner()->GetName())
		
}

// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (GetTotalMassOfActors() >= Threshold) OnOpen.Broadcast();
	else OnClose.Broadcast();
	
}

float UOpenDoor::GetTotalMassOfActors()
{
	auto TotalMass = 0.0f;

	TArray<AActor*> OverlappingActors;

	if(PressurePlate) PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	// Some up actor masses
	for (const auto* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

