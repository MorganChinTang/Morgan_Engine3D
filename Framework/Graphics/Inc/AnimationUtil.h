#pragma once

#include "ModelManager.h"

namespace Engine3D::Graphics::AnimationUtil
{
	//defininf a vector of bone matrices to use for skeleton calculations
	using BoneTransforms = std::vector<Math::Matrix4>;

	//Compute the matrices for all the bones in the hierarchy
	void ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms);
	// to be called after ComputeBoneTransforms. draws the skeketon hierarchy
	void DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms);
	//to be called to apply bone offsets for skinning data
	void ApplyBoneOffset(ModelId modelId, BoneTransforms& boneTransforms);
}