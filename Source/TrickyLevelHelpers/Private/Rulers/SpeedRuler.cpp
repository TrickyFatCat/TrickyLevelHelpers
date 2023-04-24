// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/SpeedRuler.h"

#include "Components/BillboardComponent.h"
#include "Components/DebugTextComponent.h"


ASpeedRuler::ASpeedRuler()
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
	}
#else
	PrimaryActorTick.bCanEverTick = false;
#endif
}

bool ASpeedRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ASpeedRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA
	bIsEditorOnlyActor = !bShowInGame;
	Distance = Speed * Time;

	if (DebugText)
	{
		FDebugLabelData DebugLabelData;
		DebugLabelData.Color = Color;
		DebugLabelData.TextScale = 1.15;

		const FString DistanceText = FString::Printf(TEXT("Distance : %d | %.2f m"),
		                                             static_cast<int32>(Distance),
		                                             Distance / 100.0);

		const FString Text = FString::Printf(TEXT("%s\n---------\n%s\nSpeed : %.2f m/s\nTime : %.2f sec"), *NoteText,
		                                     *DistanceText,
		                                     Speed / 100.0,
		                                     Time);
		DebugLabelData.Text = Text;
		DebugText->SetDebugLabel(DebugLabelData);
	}
#endif
}

void ASpeedRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA
	Distance = Speed * Time;

	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + GetActorForwardVector() * Distance;

	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              Color,
	              false,
	              -1,
	              0,
	              Thickness);

	if (bShowCircle)
	{
		DrawCircle(GetWorld(),
		           LineStart,
		           GetActorForwardVector(),
		           GetActorRightVector(),
		           Color,
		           Distance,
		           32,
		           false,
		           -1,
		           0,
		           Thickness);
	}
#endif
}
