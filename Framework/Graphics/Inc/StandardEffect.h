#pragma once

#include "ConstantBuffer.h"
#include "PixelShader.h"
#include "VertexShader.h"
#include "DirectionalLight.h"

namespace Engine3D::Graphics
{
	class Camera;
	class RenderObject;

	class StandardEffect final
	{
	public:
		void Initialize(const std::filesystem::path& path);
		void Terminate();

		void Begin();
		void End();

		void Render(const RenderObject& renderObject);

		void SetCamera(const Camera& camera);
		void SetDirectionalLight(const DirectionalLight& directionalLight);
		void DebugUI();

	private:
		struct TransformData
		{
			Math::Matrix4 wvp;
			Math::Matrix4 world;
			Math::Vector3 viewPosition;
			float padding = 0.0f;
		};

		using TransformBuffer = TypedConstantBuffer<TransformData>;
		TransformBuffer mTransformBuffer;

		using LightBuffer = TypedConstantBuffer<DirectionalLight>;
		LightBuffer mLightBuffer;

		VertexShader mVertexShader;
		PixelShader mPixelShader;

		const Camera* mCamera = nullptr;
		const DirectionalLight* mDirectionalLight = nullptr;
	};
}