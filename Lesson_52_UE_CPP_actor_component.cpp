#include "locationInformerComponent.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"

UlocationInformerComponent::UlocationInformerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UlocationInformerComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();

	if (Owner)
	{
		FString OwnerName = Owner->GetName();
		FVector OwnerLocation = Owner->GetActorLocation();

		FString LogMessage = FString::Printf(TEXT("Owner: %s, Location: X=%.2f, Y=%.2f, Z=%.2f"), *OwnerName, OwnerLocation.X, OwnerLocation.Y, OwnerLocation.Z);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *LogMessage);
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Yellow, LogMessage);
		}
	}
}


void UlocationInformerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
