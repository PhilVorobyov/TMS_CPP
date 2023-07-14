#include "DoorWeightComponent.h"
#include "Engine/StaticMeshActor.h"

UDoorWeightComponent::UDoorWeightComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    PrimaryComponentTick.TickInterval = 0.01f;
    TriggerVolume = nullptr;
    WeightReferenceMesh = nullptr;
}

void UDoorWeightComponent::BeginPlay()
{
    Super::BeginPlay();
    InitialRotation = GetOwner()->GetActorRotation();
    if(WeightReferenceMesh) {
        WeightReferenceMesh->OnActorBeginOverlap.AddDynamic(this, &UDoorWeightComponent::OnOverlapBegin);
        WeightReferenceMesh->OnActorEndOverlap.AddDynamic(this, &UDoorWeightComponent::OnOverlapEnd);
        TriggerVolume->OnActorBeginOverlap.AddDynamic(this, &UDoorWeightComponent::OpenDoor);

    } else {
        UE_LOG(LogTemp, Warning, TEXT("WeightReferenceMesh is not set in the DoorWeightComponent."));
    }
    
    TargetRotation = InitialRotation;
    TargetRotation.Yaw += 180.0f;

    UTextRenderComponent* TextComponent = NewObject<UTextRenderComponent>(this);
    TextComponent->RegisterComponent();
    TextComponent->SetVisibility(false); 
    TextComponent->SetWorldSize(5); 
    TextComponent->SetText(FText::FromString("")); 

}

void UDoorWeightComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    switch (CurrentDoorState)
    {
    case EDoorState::Opening:
        OpenAlpha += OpenSpeed * DeltaTime;
        if (OpenAlpha >= 0.5f)
        {
            CurrentDoorState = EDoorState::Opened;
        }
        break;
    case EDoorState::Opened:
        OpenAlpha += OpenSpeed * DeltaTime;
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
    case EDoorState::Closed:
        DisplayMessage();
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

    UTextRenderComponent* TextComponent = Cast<UTextRenderComponent>(TriggeredMessage->GetComponentByClass(UTextRenderComponent::StaticClass()));
    if (TextComponent && TextComponent->IsVisible())
    {
        APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
        if (PlayerPawn)
        {
            FVector Direction = (PlayerPawn->GetActorLocation() - TextComponent->GetComponentLocation()).GetSafeNormal();
            FRotator NewRotation = Direction.Rotation();
            NewRotation.Pitch = 0.0f; // Optional: Reset pitch to avoid the text rotating up/down.
            TextComponent->SetWorldRotation(NewRotation);
        }
    }
}

void UDoorWeightComponent::OpenDoor(AActor* OverlappedActor, AActor* OtherActor)
{
    UpdateTotalWeight();
    if (CurrentWeight >= WeightToOpen)
    {
        CurrentDoorState = EDoorState::Opening;
    }
    else
    {
        UTextRenderComponent* TextComponent = Cast<UTextRenderComponent>(TriggeredMessage->GetComponentByClass(UTextRenderComponent::StaticClass()));
        if (TextComponent)
        {
            TextComponent->SetVisibility(true);
            TextComponent->SetText(FText::FromString(Message));
            GetWorld()->GetTimerManager().SetTimer(UnusedHandle, this, &UDoorWeightComponent::HideMessage, 5.0f, false);
        }
    }
}

void UDoorWeightComponent::UpdateTotalWeight()
{
    CurrentWeight = 0.0f;
    for (AActor* Actor : OverlappingActors)
    {
        UPrimitiveComponent* ActorPrimitive = Cast<UPrimitiveComponent>(Actor->GetRootComponent());
        if (ActorPrimitive)
        {
            CurrentWeight += ActorPrimitive->GetMass();
        }
    }
}

void UDoorWeightComponent::OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor)
{
    APawn* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
    if (OtherActor && OtherActor != PlayerPawn)
    {
        OverlappingActors.AddUnique(OtherActor);
    }
}

void UDoorWeightComponent::OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor)
{
    if (OverlappingActors.Contains(OtherActor))
    {
        OverlappingActors.Remove(OtherActor);
    }
}

void UDoorWeightComponent::HideMessage()
{
    UTextRenderComponent* TextComponent = Cast<UTextRenderComponent>(TriggeredMessage->GetComponentByClass(UTextRenderComponent::StaticClass()));
    if (TextComponent)
    {
        TextComponent->SetVisibility(false);
    }
}
