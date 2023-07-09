// ActionComponent.h

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ActionComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TMS_CPP_API UActionComponent : public USceneComponent
{
	GENERATED_BODY()

public:
	UActionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION()
	void OnRightMouseButtonClicked();

protected:
	virtual void BeginPlay() override;

private:

	bool bEnableRayTrace;
	float MaxTraceDistance;
	void PerformRayTrace();
};
