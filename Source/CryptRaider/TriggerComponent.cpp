// Fill out your copyright notice in the Description page of Project Settings.

#include "TriggerComponent.h"

UTriggerComponent::UTriggerComponent()
{
    // Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
    // off to improve performance if you don't need them.
    PrimaryComponentTick.bCanEverTick = true;

    // ...
}

// Called when the game starts
void UTriggerComponent::BeginPlay()
{
    Super::BeginPlay();
    UE_LOG(LogTemp, Display, TEXT("TriggerComponent Created"));

    // ...
}

// Called every frame
void UTriggerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!Mover)
    {
        UE_LOG(LogTemp, Warning, TEXT("Mover Component not attached, skipping tick"));
        return;
    }
    AActor *Actor = GetAcceptableActor();
    if (Actor != nullptr)
    {
        UPrimitiveComponent *Component = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (Component != nullptr)
        {
            Component->SetSimulatePhysics(false);
        }
        Actor->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);
        Mover->SetShouldMove(true);
    }
    else
    {
        Mover->SetShouldMove(false);
    }
}
AActor *UTriggerComponent::GetAcceptableActor() const
{
    TArray<AActor *> Actors;
    GetOverlappingActors(Actors);
    for (AActor *Actor : Actors)
    {
        if (Actor->ActorHasTag(AcceptableActorTag) && !Actor->ActorHasTag("Grabbed"))
        {
            return Actor;
        }
    }
    return nullptr;
}

void UTriggerComponent::SetMover(UMover *NewMover)
{
    Mover = NewMover;
}