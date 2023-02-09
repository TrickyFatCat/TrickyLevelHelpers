// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "ActorOrganizerBase.h"

AActorOrganizerBase::AActorOrganizerBase()
{
	PrimaryActorTick.bCanEverTick = false;
	SetActorTickEnabled(false);

#if WITH_EDITORONLY_DATA
	
	SceneComponent = CreateEditorOnlyDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(SceneComponent);

#endif
}

void AActorOrganizerBase::OnConstruction(const FTransform& Transform)
{
	GenerateActors();
	
	Super::OnConstruction(Transform);
}

void AActorOrganizerBase::GenerateActors()
{
#if WITH_EDITORONLY_DATA
	
	if (!ChildActorClass)
	{
		return;
	}

#endif
}
