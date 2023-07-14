#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "Components/TextRenderComponent.h"
#include "DoorState.h"
#include "DoorWeightComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TMS_CPP_API UDoorWeightComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDoorWeightComponent();
	TArray<AActor*> OverlappingActors;

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "WeightDoor")
	void OpenDoor(AActor* OverlappedActor, AActor* OtherActor);
	void UpdateTotalWeight();

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UFUNCTION()
	void OnOverlapEnd(AActor* OverlappedActor, AActor* OtherActor);

	void HideMessage();

	UPROPERTY(EditAnywhere, Category = "WeightDoor")
	ATriggerVolume* TriggerVolume;

	UPROPERTY(EditAnywhere, Category = "WeightDoor")
	ATriggerVolume* WeightReferenceMesh;

	UPROPERTY(EditAnywhere, Category = "WeightDoor")
	AActor* TriggeredMessage;

	UPROPERTY(EditAnywhere, Category = "WeightDoor")
	float WeightToOpen = 50.f;

	UPROPERTY(EditAnywhere, Category = "WeightDoor")
	FString Message = "place item on scales <br> to open the door";

	UPROPERTY(BlueprintReadOnly, Category = "WeightDoor")
	float CurrentWeight = 0.f;

	UFUNCTION(BlueprintImplementableEvent, Category = "WeightDoor")
	void DisplayMessage();

private:
	FRotator InitialRotation;
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, Category = "WeightDoor")
	float OpenSpeed = 0.2f;

	float OpenAlpha = 0.0f;
	EDoorState CurrentDoorState = EDoorState::Closed;
	FTimerHandle UnusedHandle;
};
