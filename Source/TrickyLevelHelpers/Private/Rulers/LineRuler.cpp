// MIT License Copyright. Created by Artyom "Tricky Fat Cat" Volkov


#include "Rulers/LineRuler.h"

#include "Components/BillboardComponent.h"


ALineRuler::ALineRuler()
{
	Root = CreateDefaultSubobject<UBillboardComponent>("Root");
	SetRootComponent(ToRawPtr(Root));

#if WITH_EDITORONLY_DATA
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

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
	Root->SetSprite(ConstructorStatics.SpriteTexture.Object);
#else
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
#endif

	bIsEditorOnlyActor = true;
}

bool ALineRuler::ShouldTickIfViewportsOnly() const
{
	return true;
}

void ALineRuler::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

#if WITH_EDITORONLY_DATA

#endif
}

void ALineRuler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

#if WITH_EDITORONLY_DATA
	DrawLine(Distance.X, GetActorForwardVector(), FColor::Red);
	DrawLine(Distance.Y, GetActorRightVector(), FColor::Green);
	DrawLine(Distance.Z, GetActorUpVector(), FColor::Blue);

	if (bDrawMarks)
	{
		DrawMarks(Distance.X, MarksDistance.X, GetActorForwardVector(), FColor::Red);
		DrawMarks(Distance.Y, MarksDistance.Y, GetActorRightVector(), FColor::Green);
		DrawMarks(Distance.Z, MarksDistance.Z, GetActorUpVector(), FColor::Blue);
	}
#endif
}

void ALineRuler::DrawLine(const double Length, const FVector& Axis, const FColor& Color) const
{
	if (Length == 0.f)
	{
		return;
	}

	const FVector LineStart = GetActorLocation();
	const FVector LineEnd = LineStart + Axis * Length;

	DrawDebugLine(GetWorld(),
	              LineStart,
	              LineEnd,
	              Color,
	              false,
	              -1,
	              0,
	              LineThickness);
}

void ALineRuler::DrawMarks(const double Length,
                           const double MarkDistance,
                           const FVector& Axis,
                           const FColor& Color) const
{
	if (Length == 0.0 || MarkDistance == 0.0)
	{
		return;
	}

	const int32 Amount = FMath::FloorToInt32(FMath::Abs(Length) / MarkDistance);


	for (int32 i = 0; i < Amount; ++i)
	{
		const FVector Location = GetActorLocation() + Axis * MarkDistance * (i + 1) * FMath::Sign(Length);
		DrawDebugCrosshairs(GetWorld(),
		                    Location,
		                    GetActorRotation(),
		                    MarksScale,
		                    Color);
	}
}
