#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "DoorComponent.generated.h"

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed,
	Opened,
	Opening,
	Closing
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TMS_CPP_API UDoorComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UDoorComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Door")
	void OpenDoor();

	UFUNCTION()
	void OnOverlapBegin(AActor* OverlappedActor, AActor* OtherActor);

	UPROPERTY(EditAnywhere, Category = "Door")
	ATriggerVolume* TriggerVolume;

private:
	FRotator InitialRotation;
	FRotator TargetRotation;

	UPROPERTY(EditAnywhere, Category = "Door")
	float OpenSpeed = 0.2f;

	float OpenAlpha = 0.0f;
	EDoorState CurrentDoorState = EDoorState::Closed;
};
