// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (GetPhysicsHandle() && GetPhysicsHandle()->GetGrabbedComponent() != nullptr)
	{

		FVector TargetLoaction = GetComponentLocation() + GetForwardVector() * HoldDistance;
		GetPhysicsHandle()->SetTargetLocationAndRotation(TargetLoaction, GetComponentRotation());
	}
	// ...
}

void UGrabber::Grab()
{

	FHitResult HitResult;
	if (GetPhysicsHandle() && GetGrabbableInReach(HitResult))
	{
		UPrimitiveComponent *HitComponent = HitResult.GetComponent();
		HitComponent->WakeAllRigidBodies();
		HitComponent->SetSimulatePhysics(true);
		AActor *HitActor = HitResult.GetActor();
		HitActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		HitActor->Tags.Add("Grabbed");
		GetPhysicsHandle()->GrabComponentAtLocationWithRotation(HitComponent, NAME_None, HitResult.ImpactPoint, GetComponentRotation());
	}
	else
	{
		UE_LOG(LogTemp, Display, TEXT("No Actor Hit"));
	}
}

UPhysicsHandleComponent *UGrabber::GetPhysicsHandle() const
{
	UPhysicsHandleComponent *Result = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (Result == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Grabber Requires a Physics Handle Componet"));
	}
	return Result;
}
void UGrabber::ReleaseObject()
{

	if (GetPhysicsHandle() && GetPhysicsHandle()->GetGrabbedComponent() != nullptr)
	{
		GetPhysicsHandle()->GetGrabbedComponent()->WakeAllRigidBodies();
		GetPhysicsHandle()->GetGrabbedComponent()->GetOwner()->Tags.Remove("Grabbed");

		GetPhysicsHandle()->ReleaseComponent();
	}
}

bool UGrabber::GetGrabbableInReach(FHitResult &OutHit) const
{

	FVector Start = GetComponentLocation();
	FVector End = Start + GetForwardVector() * MaxGrabDistance;

	DrawDebugLine(GetWorld(), Start, End, FColor::Red);
	DrawDebugSphere(GetWorld(), End, 10, 10, FColor::Blue, false, 5);

	FCollisionShape Sphere = FCollisionShape::MakeSphere(GrabRadius);
	return GetWorld()->SweepSingleByChannel(OutHit, Start, End, FQuat::Identity, ECC_GameTraceChannel2, Sphere);
}