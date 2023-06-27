#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "AnimationCoreLibrary.h"
#include "FixDAZTwistBones.generated.h"


USTRUCT(BlueprintType)
struct FBendBone
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Bone name"), Category = "Setup")
	FName BoneName;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation offset"), Category = "Setup")
	float Offset;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation order"), Category = "Setup")
	EEulerRotationOrder RotationOrder;
};

UCLASS()
class FREEANIMHELPERSEDITOR_API UFixDAZTwistBones : public UAnimationModifier
{
	GENERATED_BODY()

public:
	UFixDAZTwistBones();

	// Order matters for bones in hierarchy
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Bend bone settings"), Category = "Setup")
	TMap<FName, FBendBone> TwistToBend;

	/* UAnimationModifier overrides */
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	/* UAnimationModifier overrides end */
};

