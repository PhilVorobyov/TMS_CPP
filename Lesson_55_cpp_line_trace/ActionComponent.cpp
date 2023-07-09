
#include "ActionComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"

UActionComponent::UActionComponent()
{
    bEnableRayTrace = false;
    MaxTraceDistance = 10000.f;

    PrimaryComponentTick.bCanEverTick = true;
}

void UActionComponent::BeginPlay()
{
    Super::BeginPlay();
    SetComponentTickEnabled(true);

    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (PlayerController)
    {
        PlayerController->InputComponent->BindAction("RightMouseButton", IE_Pressed, this, &UActionComponent::OnRightMouseButtonClicked);
    }
}

void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    if (bEnableRayTrace)
    {
        PerformRayTrace();
    }
}

void UActionComponent::OnRightMouseButtonClicked()
{
    bEnableRayTrace = !bEnableRayTrace;
}

void UActionComponent::PerformRayTrace()
{
    APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    if (!PlayerController)
        return;

    APawn* ControlledPawn = PlayerController->GetPawn();
    if (!ControlledPawn)
        return;

    USceneComponent* FirstPersonCamera = ControlledPawn->FindComponentByClass<UCameraComponent>();
    if (!FirstPersonCamera)
        return;
    
    FVector StartLocation = FirstPersonCamera->GetComponentLocation();
    FVector EndLocation = StartLocation + FirstPersonCamera->GetForwardVector() * MaxTraceDistance;
    
    FHitResult HitResult;
    FCollisionQueryParams TraceParams(FName(TEXT("RayTrace")), true, ControlledPawn);
    GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

    if (HitResult.bBlockingHit)
    {
        FVector HitLocation = HitResult.ImpactPoint;
        FRotator HitNormal = HitResult.ImpactNormal.Rotation();

        AActor* HitActor = HitResult.GetActor();
        if (HitActor)
        {
            FString ActorName = HitActor->GetName();
            UE_LOG(LogTemp, Warning, TEXT("Interacted with actor: %s"), *ActorName);
        }

        DrawDebugLine(GetWorld(), StartLocation, HitLocation, FColor::Red, false, -1.f, 0, 1.f);
        DrawDebugPoint(GetWorld(), HitLocation, 10.f, FColor::Red, false, -1.f);
        DrawDebugLine(GetWorld(), HitLocation, HitLocation + HitNormal.Vector() * 50.f, FColor::Green, false, -1.f, 0, 1.f);
    }
}
