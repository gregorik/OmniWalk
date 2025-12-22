#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class UOmniWalkComponent;

/**
 * SOmniWalkDebugger
 * Marked with OMNIWALKEDITOR_API to resolve Linker (LNK2019) visibility.
 */
class OMNIWALKEDITOR_API SOmniWalkDebugger : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SOmniWalkDebugger) {}
	SLATE_END_ARGS()

	/** Constructs the Slate widget */
	void Construct(const FArguments& InArgs);

private:
	TSharedRef<SWidget> CreateAttributeRow(FText Label, TAttribute<FText> ValueBinding);

	UOmniWalkComponent* GetActiveComponent() const;

	FText GetAdhesionStatus() const;
	FText GetGravityVectorText() const;
	TOptional<float> GetAlignmentStrength() const;
};