#pragma once

#include "MeshTypes.h"
#include "Material.h"
#include "Skeleton.h"
#include "AnimationClip.h"

namespace Engine3D::Graphics
{
	struct Model
	{
		struct MeshData
		{
			Mesh mesh;
			uint32_t materialIndex;
		};

		struct MaterialData
		{
			Material material;
			std::string diffuseMapName;
			std::string specMapName;
			std::string normalMapName;
			std::string bumpMapName;

		};

		std::vector<MeshData> meshData;
		std::vector<MaterialData> materialData;
		std::unique_ptr<Skeleton> skeleton;
		std::vector<AnimationClip> animationClips;
	};
}