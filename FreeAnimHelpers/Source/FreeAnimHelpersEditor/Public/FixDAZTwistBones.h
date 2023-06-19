#pragma once

#include "CoreMinimal.h"
#include "AnimationModifier.h"
#include "FixDAZTwistBones.generated.h"

UENUM(BlueprintType)
enum class TwistAxis : uint8 {
	X = 0 UMETA(DisplayName = "X"),
	Y = 1  UMETA(DisplayName = "Y"),
};

USTRUCT(BlueprintType)
struct FBendBone
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Bone name"), Category = "Setup")
	FName BoneName;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Rotation offset"), Category = "Setup")
	float Offset;

	UPROPERTY(EditAnywhere, meta = (DisplayName = "Twist axis"), Category = "Setup")
	TwistAxis Axis;
};

UCLASS()
class FREEANIMHELPERSEDITOR_API UFixDAZTwistBones : public UAnimationModifier
{
	GENERATED_BODY()

public:
	UFixDAZTwistBones();

	// Order matters for bones in hierarchy
	UPROPERTY(EditAnywhere, meta = (DisplayName = "Twist bone to bend bone"), Category = "Setup")
		TMap<FName, FBendBone> TwistToBend;

	/* UAnimationModifier overrides */
	virtual void OnApply_Implementation(UAnimSequence* AnimationSequence) override;
	/* UAnimationModifier overrides end */
};

