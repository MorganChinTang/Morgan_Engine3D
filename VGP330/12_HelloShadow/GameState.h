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

	void UpdateCamera(float deltaTime);

	Engine3D::Graphics::Camera mCamera;
	Engine3D::Graphics::DirectionalLight mDirectionalLight;

	Engine3D::Graphics::RenderGroup mCharacter1;
	Engine3D::Graphics::RenderGroup mCharacter2;
	Engine3D::Graphics::RenderGroup mCharacter3;

	Engine3D::Graphics::RenderObject mSphere1;
	Engine3D::Graphics::RenderObject mSphere2;

	Engine3D::Graphics::RenderObject mGround;
	
	Engine3D::Graphics::StandardEffect mStandardEffect;
	Engine3D::Graphics::ShadowEffect mShadowEffect;
};