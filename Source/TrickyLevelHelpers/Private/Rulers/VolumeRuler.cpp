// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/VolumeRuler.h"

#include "Components/BillboardComponent.h"


AVolumeRuler::AVolumeRuler()
{
	bIsEditorOnlyActor = true;

#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateEditorOnlyDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);
	
	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Billboard");
	Billboard->SetupAttachment(GetRootComponent());
	
	struct FConstructorStatics
	{
		ConstructorHelpers::FObjectFinder<UTexture2D> SpriteTexture;
		FName ID_Misc;
		FText NAME_Misc;

		FConstructorStatics()
			: SpriteTexture(TEXT("/Engine/EditorResources/S_Note"))
			  , ID_Misc(TEXT("Misc"))
			  , NAME_Misc(NSLOCTEXT("SpriteCategory", "Misc", "Misc"))
		{
		}
	};

	static FConstructorStatics ConstructorStatics;
	Billboard->SetSprite(ConstructorStatics.SpriteTexture.Object);
	Billboard->bUseInEditorScaling = false;
	Billboard->bIsScreenSizeScaled = true;
	Billboard->ScreenSize = 0.001;
	Color.A = 255 * 0.15;

#else
	PrimaryActorTick.bCanEverTick = false;
#endif
}

bool AVolumeRuler::ShouldTickIfViewportsOnly() const
{
#if WITH_EDITORONLY_DATA
	return true;
#else
	return Super::ShouldTickIfViewportsOnly();
#endif
}

void AVolumeRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
#if WITH_EDITORONLY_DATA
	Color.A = 255 * 0.15;
	Extent = Size * 0.5;
	Center = bCenterOrigin ? GetActorLocation() : GetActorLocation() + Extent;
#endif
}

void AVolumeRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA

	DrawDebugBox(GetWorld(),
	             Center,
	             Extent,
	             Color,
	             false,
	             -1,
	             0,
	             Thickness);

	if (bDrawSolidBox)
	{
		DrawDebugSolidBox(GetWorld(),
		                  Center,
		                  Extent - 0.1,
		                  FRotator::ZeroRotator.Quaternion(),
		                  Color,
		                  false,
		                  -1,
		                  0);
	}
#endif
}
