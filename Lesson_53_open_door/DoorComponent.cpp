#include "DoorComponent.h"

UDoorComponent::UDoorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.01f;
    TriggerVolume = CreateDefaultSubobject<ATriggerVolume>(TEXT("TriggerVolume"));
}

void UDoorComponent::BeginPlay()
{
    Super::BeginPlay();
    InitialRotation = GetOwner()->GetActorRotation();
    TriggerVolume->OnActorBeginOverlap.AddDynamic(this, &UDoorComponent::OnOverlapBegin);
}

void UDoorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (CurrentDoorState)
    {
    case EDoorState::Opening:
        OpenAlpha += OpenSpeed * DeltaTime;;
        if (OpenAlpha >= 0.5f)
        {
            CurrentDoorState = EDoorState::Opened;
        }
        break;
    case EDoorState::Opened:
        OpenAlpha += OpenSpeed * DeltaTime;;
        if (OpenAlpha >= 1.0f)
        {
            OpenAlpha = 0.5f;
            CurrentDoorState = EDoorState::Closing;
        }
        break;
    case EDoorState::Closing:
        OpenAlpha -= OpenSpeed * DeltaTime;
        if (OpenAlpha <= 0.0f)
        {
            OpenAlpha = 0.0f;
            CurrentDoorState = EDoorState::Closed;
        }
        break;
    default:
        break;
    }

    if (CurrentDoorState  == EDoorState::Opening)
    {
        FRotator NewRotation = FMath::Lerp(InitialRotation, TargetRotation, OpenAlpha);
        GetOwner()->SetActorRotation(NewRotation);
    }
    else if (CurrentDoorState  == EDoorState::Closing)
    {
        FRotator NewRotation = FMath::Lerp(InitialRotation, TargetRotation, OpenAlpha);
        GetOwner()->SetActorRotation(NewRotation);
    }
    
}

void UDoorComponent::OpenDoor()
{
    if (CurrentDoorState != EDoorState::Opening)
    {
        CurrentDoorState = EDoorState::Opening;
    }
}

void UDoorComponent::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
    {
        OpenDoor();
    }
}
