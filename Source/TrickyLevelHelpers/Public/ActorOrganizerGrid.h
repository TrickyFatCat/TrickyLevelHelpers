// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov

#pragma once

#include "CoreMinimal.h"
#include "ActorOrganizerBase.h"
#include "ActorOrganizerGrid.generated.h"

/**
 * 
 */
UCLASS()
class TRICKYLEVELHELPERS_API AActorOrganizerGrid : public AActorOrganizerBase
{
	GENERATED_BODY()

protected:
	virtual void GenerateActors() override;
	
#if WITH_EDITORONLY_DATA
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FGridSize GridSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FVector LocationOffset{64.f, 64.f, 0.f};

#endif
};
