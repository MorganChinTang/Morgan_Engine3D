#pragma once

#include "MeshBuffer.h"
#include "Transform.h"
#include "Material.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "Animator.h"

namespace Engine3D::Graphics
{
	struct Skeleton;

	class RenderObject
	{
	public:
		void Terminate();

		Transform transform;
		MeshBuffer meshBuffer;
		Material material;
		TextureId diffuseMapId;
		TextureId specMapId;
		TextureId normalMapId;
		TextureId bumpMapId;

	};
	class RenderGroup
	{
	public:
		void Initialize(const std::filesystem::path& modelFilePath);
		void Terminate();

		ModelId modelId;
		Transform transform;
		std::vector<RenderObject> renderObjects;

		const Skeleton* skeleton = nullptr;
		const Animator* animator = nullptr;
	};
}