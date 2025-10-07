#pragma once

#include "MeshBuffer.h"
#include "Transform.h"

namespace Engine3D::Graphics
{
	class RenderObject
	{
	public:
		void Terminate();

		Transform transform;
		MeshBuffer meshBuffer;
	};
}