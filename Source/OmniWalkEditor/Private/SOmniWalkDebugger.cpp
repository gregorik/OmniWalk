#include "SOmniWalkDebugger.h"
#include "OmniWalkComponent.h"
#include "OmniWalkStyle.h"
// Engine Headers
#include "Widgets/Layout/SScrollBox.h"
#include "Widgets/Layout/SBorder.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Notifications/SProgressBar.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Editor.h"

#define LOCTEXT_NAMESPACE "SOmniWalkDebugger"

void SOmniWalkDebugger::Construct(const FArguments& InArgs)
{
	const FMargin ContentPadding = FMargin(10.f, 5.f);

	ChildSlot
		[
			SNew(SBorder)
				.BorderImage(FAppStyle::GetBrush("ToolPanel.GroupBorder"))
				[
					SNew(SScrollBox)
						+ SScrollBox::Slot()
						[
							SNew(SVerticalBox)

								+ SVerticalBox::Slot().AutoHeight().Padding(ContentPadding)
								[
									SNew(STextBlock)
										.Text(LOCTEXT("DebuggerTitle", "OmniWalk Telemetry"))
										.Font(FAppStyle::GetFontStyle("SettingsEditor.SetttingsHeader"))
										.ColorAndOpacity(FLinearColor(0.2f, 0.7f, 1.0f))
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(ContentPadding)
								[
									CreateAttributeRow(LOCTEXT("StatusLabel", "Adhesion:"),
										TAttribute<FText>::Create(TAttribute<FText>::FGetter::CreateRaw(this, &SOmniWalkDebugger::GetAdhesionStatus)))
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(ContentPadding)
								[
									SNew(STextBlock).Text_Raw(this, &SOmniWalkDebugger::GetGravityVectorText)
								]

								+ SVerticalBox::Slot().AutoHeight().Padding(ContentPadding)
								[
									SNew(SProgressBar)
										.Percent_Raw(this, &SOmniWalkDebugger::GetAlignmentStrength)
										.FillColorAndOpacity(FLinearColor::Green)
								]
						]
				]
		];
}

TSharedRef<SWidget> SOmniWalkDebugger::CreateAttributeRow(FText Label, TAttribute<FText> ValueBinding)
{
	return SNew(SHorizontalBox)
		+ SHorizontalBox::Slot().AutoWidth()
		[
			SNew(STextBlock).Text(Label).Font(FAppStyle::GetFontStyle("DetailsView.CategoryFontStyle"))
		]
		+ SHorizontalBox::Slot().FillWidth(1.f).HAlign(HAlign_Right)
		[
			SNew(STextBlock).Text(ValueBinding)
		];
}

UOmniWalkComponent* SOmniWalkDebugger::GetActiveComponent() const
{
	if (!GEditor) return nullptr;
	FWorldContext* Context = GEditor->GetPIEWorldContext();
	UWorld* World = Context ? Context->World() : nullptr;
	if (!World) return nullptr;

	APlayerController* PC = World->GetFirstPlayerController();
	APawn* Pawn = PC ? PC->GetPawn() : nullptr;
	return Pawn ? Pawn->FindComponentByClass<UOmniWalkComponent>() : nullptr;
}

FText SOmniWalkDebugger::GetAdhesionStatus() const
{
	UOmniWalkComponent* Comp = GetActiveComponent();
	return (Comp && Comp->IsActive()) ? LOCTEXT("Active", "LOCKED") : LOCTEXT("Inactive", "SEARCHING...");
}

FText SOmniWalkDebugger::GetGravityVectorText() const
{
	UOmniWalkComponent* Comp = GetActiveComponent();
	if (Comp && Comp->GetOwner())
	{
		if (ACharacter* Char = Cast<ACharacter>(Comp->GetOwner()))
		{
			if (UCharacterMovementComponent* CMC = Char->GetCharacterMovement())
			{
				FVector Grav = CMC->GetGravityDirection();
				return FText::FromString(FString::Printf(TEXT("Gravity: X:%0.2f Y:%0.2f Z:%0.2f"), Grav.X, Grav.Y, Grav.Z));
			}
		}
	}
	return LOCTEXT("NoComp", "Gravity: N/A");
}

TOptional<float> SOmniWalkDebugger::GetAlignmentStrength() const
{
	UOmniWalkComponent* Comp = GetActiveComponent();
	if (Comp && Comp->GetOwner())
	{
		if (ACharacter* Char = Cast<ACharacter>(Comp->GetOwner()))
		{
			if (UCharacterMovementComponent* CMC = Char->GetCharacterMovement())
			{
				return FMath::Max(0.0f, FVector::DotProduct(Char->GetActorUpVector(), -CMC->GetGravityDirection()));
			}
		}
	}
	return 0.0f;
}

#undef LOCTEXT_NAMESPACE