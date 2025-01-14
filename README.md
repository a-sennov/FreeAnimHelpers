# FreeAnimHelpers
Unreal Engine 5.2 plugin with some animation helpers.

Currenty, it contains four useful animation helpers.

* AnimateIKBones - snap IK bones to their FK parts.
* FixDAZTwistBones - useful when you are retargetting or importing animation from software that doesn't support DAZ-style child twist bones (like Cascadeur).
* ResetScale - reset scale of root bone to 1,0 but preserve size of skeletal mesh
* ResetBoneTranslation - reset translation pf bpnes that were moved during retargetting.

## Usage

Copy *FreeAnimHelpers* folder to [your project]/Plugins or Engine/Plugins and enable it in Project Settings -> Plugins window.

### SnapFootToGround (Animation Modifier)

Modifier to snap character feet to ground. I use it for turn-in-place animation like [this](https://www.youtube.com/watch?v=TX2gcdWHLpY).

1. Open Skeletal Mesh used in animation you need to modify.

2. For both legs, add tip sockets:
- attached to foot bone;
- located at ball and at the ground, orientation doesn't matter.

![Tip socket placement](readme_tip.jpg)

3. Open animation sequence asset in Animation Editor, then Animation Data Modifiers window (via Windows menu) and add **SnapFootToGround** modifier.

4. Set *Snap Foot Orientation* checkbox, if you want to make feet horizontal.

5. Fill names of feet bones and tip sockets, then right click and select *Apply Modifier*.

6. Save animation sequence.

### PrepareTurnInPlaceAsset (Animation Modifier)

For my personal specific puropses. You don't need it.

### ResetBonesTranslation (Animation Modifier)

For all bones with "Translation Retargeting Option" = "Skeleton" in the skeleton hierarchy, this modifier changes local translation in animation sequence to skeleton-default. In other words, after thes modifier you can reset "Translation Retargeting Option" for all bones back to "Animation". Useful if you want to export to FBX animation sequence retargeted from another skeleton.

Usage: add modifier to animation sequence, select desired skeletal mesh (to get local translations of bones) and apply it.

### Animate IK Bones

Copy component-space transforms of skeleton bones (usually wrists and feet) to MetaHuamn/Mannequin IK bones (ik_foot_r etc).

### Reset Scale of Root Bone

Small function to reset scale of root bone in skeletal mesh and preserve size of the model. See [this video](https://youtu.be/o04BHS8e_M4).

## To Do

- add/remove root motion;
- insert T-pose/reference pose in the first frame of animation sequence;
- animate twist bones;
