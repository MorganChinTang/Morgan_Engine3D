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
	struct SkySphere
	{
		float radius;
		float distanceFromSun;
		float orbitSpeed;
		float rotSpeed;
		Engine3D::Graphics::SimpleTextureEffect::RenderData renderData;
	};
	void UpdateCamera(float deltaTime);

	Engine3D::Graphics::Camera mCamera;
	Engine3D::Graphics::DirectionalLight mDirectionalLight;

	Engine3D::Graphics::RenderGroup mCharacter1;
	Engine3D::Graphics::RenderGroup mCharacter2;
	Engine3D::Graphics::RenderGroup mCharacter3;

	Engine3D::Graphics::RenderObject mSphere1;
	Engine3D::Graphics::RenderObject mSphere2;

	Engine3D::Graphics::RenderObject mGround;
	Engine3D::Graphics::Terrain mTerrain;
	
	Engine3D::Graphics::StandardEffect mStandardEffect;
	Engine3D::Graphics::ShadowEffect mShadowEffect;
	Engine3D::Graphics::TerrainEffect mTerrainEffect;
	Engine3D::Graphics::DissolveEffect mDissolveEffect;
	Engine3D::Graphics::SimpleTextureEffect mSimpleTextureEffect;

	SkySphere mSkySphere;
	//render target
	Engine3D::Graphics::RenderTarget mRenderTarget;
	Engine3D::Graphics::Camera mRenderTargetCamera;

	uint32_t mCharacter1DiffuseMapId;
	uint32_t mCharacter1SpecMapId;
	uint32_t mCharacter2DiffuseMapId;
	uint32_t mCharacter2SpecMapId;
	uint32_t mCharacter3DiffuseMapId;
	uint32_t mCharacter3SpecMapId;
};