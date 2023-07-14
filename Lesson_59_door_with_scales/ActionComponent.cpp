// ActionComponent.cpp

#include "ActionComponent.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"
#include "Components/PrimitiveComponent.h"
#include "Camera/CameraComponent.h"

UActionComponent::UActionComponent()
{
	bEnableRayTrace = false;
	bIsCarryingObject = false;
	CarriedObject = nullptr;
	ThrowForce = 1000.f;
	PickupRadius = 6000.f;
	CarryRadius = 300.f;
	MaxObjectWeight = 1000.f;
	MaxTraceDistance = 6000.f;

	PrimaryComponentTick.bCanEverTick = true;
}

void UActionComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

void UActionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (bEnableRayTrace)
	{
		PerformRayTrace();
	}

	if (bIsCarryingObject && CarriedObject)
	{
		APawn* ControlledPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
		if (!ControlledPawn)
			return;

		USceneComponent* FirstPersonCamera = ControlledPawn->FindComponentByClass<UCameraComponent>();
		if (!FirstPersonCamera)
			return;

		UPrimitiveComponent* CarriedPrimitive = CarriedObject->FindComponentByClass<UPrimitiveComponent>();
		if (!CarriedPrimitive)
			return;

		FBoxSphereBounds LocalBounds = CarriedPrimitive->CalcBounds(CarriedPrimitive->GetComponentTransform());

		float Volume = 8 * LocalBounds.BoxExtent.X * LocalBounds.BoxExtent.Y * LocalBounds.BoxExtent.Z; // 8 = 2*2*2
		float Density = .0002f;
		float Mass = Volume * Density;
    
		float InterpolationSpeed = FMath::Clamp( 400.f / Mass, 0.2f, 5000.f); 

		FVector CurrentLocation = CarriedObject->GetActorLocation();
		FVector DesiredLocation = GetOwner()->GetActorLocation() + FirstPersonCamera->GetForwardVector() * CarryRadius;
		FVector NewLocation = FMath::VInterpTo(CurrentLocation, DesiredLocation, DeltaTime, InterpolationSpeed);
		CarriedObject->SetActorLocation(NewLocation);
	}
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

void UActionComponent::EnableRayTrace()
{
	bEnableRayTrace = !bEnableRayTrace;
}


void UActionComponent::ToggleObjectPickup()
{
	if (bIsCarryingObject)
	{
		ThrowObject();
	}
	else if (CarriedObject == nullptr)
	{
		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (!PlayerController)
			return;

		FVector StartLocation = PlayerController->PlayerCameraManager->GetCameraLocation();
		FVector EndLocation = StartLocation + PlayerController->PlayerCameraManager->GetCameraRotation().Vector() * PickupRadius;

		FHitResult HitResult;
		FCollisionQueryParams TraceParams(FName(TEXT("RayTrace")), true, GetOwner());
		GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, TraceParams);

		if (HitResult.bBlockingHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && HitActor != GetOwner())
			{
				UPrimitiveComponent* HitPrimitive = HitActor->FindComponentByClass<UPrimitiveComponent>();
				if (HitPrimitive->Mobility == EComponentMobility::Movable && HitPrimitive && HitPrimitive->GetMass() <= MaxObjectWeight)
				{
					if (IsValidObject(HitActor))
					{
						bIsCarryingObject = true;
						CarriedObject = HitActor;
						HitPrimitive->SetSimulatePhysics(false);
						HitPrimitive->SetGenerateOverlapEvents(true);
						HitPrimitive->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
					}
				}
			}
		}
	}
}


void UActionComponent::ThrowObject()
{
	if (CarriedObject)
	{
		UPrimitiveComponent* HitPrimitive = CarriedObject->FindComponentByClass<UPrimitiveComponent>();
		if (HitPrimitive)
		{
			HitPrimitive->SetSimulatePhysics(true);
			FVector ThrowDirection = GetOwner()->GetActorForwardVector();
			float MassAdjustedForce = ThrowForce / (HitPrimitive->GetMass() * MassMultiplier);
			FVector ThrowForceVector = ThrowDirection * MassAdjustedForce;
			HitPrimitive->AddImpulse(ThrowForceVector, NAME_None, true);
			bIsCarryingObject = false;
			CarriedObject = nullptr;
			HitPrimitive->SetSimulatePhysics(true);
		}
	}
}

bool UActionComponent::IsValidObject(AActor* Actor)
{
	return true;
}