// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/VolumeRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


AVolumeRuler::AVolumeRuler()
{
	bIsEditorOnlyActor = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateEditorOnlyDefaultSubobject<UBillboardComponent>("Billboard");

	if (Billboard)
	{
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
		SpriteScale = 0.5;
	}

	DebugText = CreateEditorOnlyDefaultSubobject<UDebugTextComponent>("DebugText");

	if (DebugText)
	{
		DebugText->SetupAttachment(GetRootComponent());
		DebugText->SetDrawOneLabel(true);
	}
#else
	PrimaryActorTick.bCanEverTick = false;
#endif
}

bool AVolumeRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void AVolumeRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	
	bIsEditorOnlyActor = !bShowInGame;
	Center = bCenterOrigin ? GetActorLocation() : GetActorLocation() + Extent;
	
	if (DebugText)
	{
		DebugText->SetDrawInGame(bShowInGame);

		Color.A = 255 * 0.15;
		Extent = Size * 0.5;
		Center = bCenterOrigin ? GetActorLocation() : GetActorLocation() + Extent;

		FDebugLabelData DebugLabelData;
		FColor TextColor = Color;
		TextColor.A = 255;
		DebugLabelData.Color = TextColor;
		DebugLabelData.TextScale = 1.15;
		DebugLabelData.bUseCustomLocation = true;
		DebugLabelData.Location = GetActorLocation();

		const FString DimensionsText = FString::Printf(TEXT("%s\n%s\n%s"),
		                                               *GetDimension(Size.X, "X"),
		                                               *GetDimension(Size.Y, "Y"),
		                                               *GetDimension(Size.Z, "Z"));
		const FString LengthData = FString::Printf(TEXT("%s\n---------\n%s"), *NoteText, *DimensionsText);
		DebugLabelData.Text = LengthData;
		DebugText->SetDebugLabel(DebugLabelData);
	}

#endif
}

void AVolumeRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA
	Extent = Size * 0.5f;
	Center = bCenterOrigin ? GetActorLocation() : GetActorLocation() + Extent;

	DrawDebugBox(GetWorld(),
	             Center,
	             Extent,
	             GetActorRotation().Quaternion(),
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
		                  GetActorRotation().Quaternion(),
		                  Color,
		                  false,
		                  -1,
		                  0);
	}
#endif
}

FString AVolumeRuler::GetDimension(const double Length, const FString& AxisName)
{
	return FString::Printf(TEXT("%s : %d | %.2f m"), *AxisName, static_cast<int32>(Length), Length / 100.0);
}
