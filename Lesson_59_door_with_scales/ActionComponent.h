// ActionComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ActionComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class TMS_CPP_API UActionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UActionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void EnableRayTrace();

	UFUNCTION()
	void ThrowObject();

	UFUNCTION()
	void ToggleObjectPickup();

	UPROPERTY(EditAnywhere)
	float ThrowForce;

	UPROPERTY(EditAnywhere)
	float MassMultiplier;

protected:
	virtual void BeginPlay() override;

private:
	bool bEnableRayTrace;
	bool bIsCarryingObject;
	AActor* CarriedObject;
	
	float PickupRadius;
	float CarryRadius;
	float MaxObjectWeight;
	float MaxTraceDistance;

	void PerformRayTrace();
	static bool IsValidObject(AActor* Actor);
};
