#pragma once

#include <3DEngine/Inc/3DEngine.h>


class GameState : public Engine3D::AppState
{
public:
	void Initialize() override;
    void Terminate() override;
	void Update(float deltaTime) override;
	void Render() override;
	void DebugUI() override;

private:
	struct Object
	{
		Engine3D::Math::Matrix4 matWorld = Engine3D::Math::Matrix4::Identity;
		Engine3D::Graphics::MeshBuffer meshBuffer;
		Engine3D::Graphics::TextureId textureId = 0;
	};

	void UpdateCamera(float deltaTime);
	void RenderObject(const Object& object, const Engine3D::Graphics::Camera& camera);

	Engine3D::Graphics::Camera mCamera;
	Engine3D::Graphics::Camera mRenderTargetCamera;

	// GPU communication
	Engine3D::Graphics::ConstantBuffer mTransformBuffer;
	Engine3D::Graphics::VertexShader mVertexShader;
	Engine3D::Graphics::PixelShader mPixelShader;
	Engine3D::Graphics::Sampler mSampler;

	// render object
	Object mObject0;
	Object mObject1;

	// render target
	Engine3D::Graphics::RenderTarget mRenderTarget;
};