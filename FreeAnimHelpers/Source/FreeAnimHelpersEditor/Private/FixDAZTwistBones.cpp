#include "FixDAZTwistBones.h"
#include "Animation/AnimData/AnimDataModel.h"
#include "Animation/AnimData/IAnimationDataController.h"
#include "FreeAnimHelpersLibrary.h"
#include "Animation/AnimSequence.h"
#include "Animation/AnimTypes.h"
#include "ReferenceSkeleton.h"
#include "Engine/SkeletalMesh.h"
#include "Engine/SkeletalMeshSocket.h"


UFixDAZTwistBones::UFixDAZTwistBones()
{
	if (TwistToBend.Num() == 0)
	{
		TwistToBend.Add(TEXT("lShldrTwist"), FBendBone{ TEXT("lShldrBend"), 3.9f, EEulerRotationOrder::XYZ });
		TwistToBend.Add(TEXT("rShldrTwist"), FBendBone{ TEXT("rShldrBend"), 3.9f, EEulerRotationOrder::XYZ });
		TwistToBend.Add(TEXT("lForearmTwist"), FBendBone{ TEXT("lForearmBend"), -0.02f, EEulerRotationOrder::XZY });
		TwistToBend.Add(TEXT("rForearmTwist"), FBendBone{ TEXT("rForearmBend"), -0.02f, EEulerRotationOrder::XZY });
		TwistToBend.Add(TEXT("lThighTwist"), FBendBone{ TEXT("lThighBend"), -13.0f, EEulerRotationOrder::YZX });
		TwistToBend.Add(TEXT("rThighTwist"), FBendBone{ TEXT("rThighBend"), 13.0f, EEulerRotationOrder::YZX });
	}
}

void UFixDAZTwistBones::OnApply_Implementation(UAnimSequence* AnimationSequence)
{
	USkeleton* Skeleton = AnimationSequence->GetSkeleton();
	const FReferenceSkeleton& RefSkeleton = Skeleton->GetReferenceSkeleton();

	const int32 KeysNum = AnimationSequence->GetDataModel()->GetNumberOfKeys();

	// Tracks to save data
	TMap<FName, FRawAnimSequenceTrack> OutTracks;

	// Initialize containers
	for (const auto& BonePair : TwistToBend)
	{
		if (RefSkeleton.FindBoneIndex(BonePair.Key) == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't find twist bone: %s"), *BonePair.Key.ToString());
			return;
		}
		if (RefSkeleton.FindBoneIndex(BonePair.Value.BoneName) == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("Can't find bend bone: %s"), *BonePair.Value.BoneName.ToString());
			return;
		}

		FRawAnimSequenceTrack& tTrack = OutTracks.Add(BonePair.Key);
		tTrack.PosKeys.SetNumUninitialized(KeysNum);
		tTrack.RotKeys.SetNumUninitialized(KeysNum);
		tTrack.ScaleKeys.SetNumUninitialized(KeysNum);

		FRawAnimSequenceTrack& bTrack = OutTracks.Add(BonePair.Value.BoneName);
		bTrack.PosKeys.SetNumUninitialized(KeysNum);
		bTrack.RotKeys.SetNumUninitialized(KeysNum);
		bTrack.ScaleKeys.SetNumUninitialized(KeysNum);
	}

	for (int32 FrameIndex = 0; FrameIndex < KeysNum; FrameIndex++)
	{
		for (const auto& BonePair : TwistToBend)
		{
			const FName& TwistBone = BonePair.Key;
			const FTransform TwistBoneTr = AnimationSequence->GetDataModel()->EvaluateBoneTrackTransform(TwistBone, FrameIndex, EAnimInterpolationType::Linear);
			const FVector twistEuler = AnimationCore::EulerFromQuat(TwistBoneTr.GetRotation(), BonePair.Value.RotationOrder);

			const FName& BendBone = BonePair.Value.BoneName;
			const FTransform BendBoneTr = AnimationSequence->GetDataModel()->EvaluateBoneTrackTransform(BendBone, FrameIndex, EAnimInterpolationType::Linear);
			const FVector bendEuler = AnimationCore::EulerFromQuat(BendBoneTr.GetRotation(), BonePair.Value.RotationOrder);

			FVector outTwistEuler;
			FVector outBendEuler;

			switch (BonePair.Value.RotationOrder) {
			case EEulerRotationOrder::XYZ:
			case EEulerRotationOrder::XZY:
				outTwistEuler = FVector(bendEuler.X + twistEuler.X - BonePair.Value.Offset, twistEuler.Y, twistEuler.Z);
				outBendEuler = FVector(BonePair.Value.Offset, bendEuler.Y, bendEuler.Z);
				break;
			case EEulerRotationOrder::YXZ:
			case EEulerRotationOrder::YZX:
				outTwistEuler = FVector(twistEuler.X, bendEuler.Y + twistEuler.Y - BonePair.Value.Offset, twistEuler.Z);
				outBendEuler = FVector(bendEuler.X, BonePair.Value.Offset, bendEuler.Z);
				break;
			case EEulerRotationOrder::ZXY:
			case EEulerRotationOrder::ZYX:
				outTwistEuler = FVector(twistEuler.X, twistEuler.Y, bendEuler.Z + twistEuler.Z - BonePair.Value.Offset);
				outBendEuler = FVector(bendEuler.X, bendEuler.Y, BonePair.Value.Offset);
				break;
			}

			FQuat outTwist = AnimationCore::QuatFromEuler(outTwistEuler, BonePair.Value.RotationOrder);
			FQuat outBend = AnimationCore::QuatFromEuler(outBendEuler, BonePair.Value.RotationOrder);

			// Save to track
			OutTracks[TwistBone].PosKeys[FrameIndex] = (FVector3f)TwistBoneTr.GetTranslation();
			OutTracks[TwistBone].RotKeys[FrameIndex] = FQuat4f(outTwist);
			OutTracks[TwistBone].ScaleKeys[FrameIndex] = (FVector3f)TwistBoneTr.GetScale3D();

			OutTracks[BendBone].PosKeys[FrameIndex] = (FVector3f)BendBoneTr.GetTranslation();
			OutTracks[BendBone].RotKeys[FrameIndex] = FQuat4f(outBend);
			OutTracks[BendBone].ScaleKeys[FrameIndex] = (FVector3f)BendBoneTr.GetScale3D();

		}
	}

	// Save new keys in DataModel
	IAnimationDataController& Controller = AnimationSequence->GetController();
	for (const auto& Track : OutTracks)
	{
		const FName& BoneName = Track.Key;
		Controller.RemoveBoneTrack(BoneName);
		if (!Controller.AddBoneCurve(BoneName)) {
			UE_LOG(LogTemp, Warning, TEXT("Failed to create bone track: %s"), *BoneName.ToString());
		}
		Controller.SetBoneTrackKeys(BoneName, Track.Value.PosKeys, Track.Value.RotKeys, Track.Value.ScaleKeys);
	}
}

