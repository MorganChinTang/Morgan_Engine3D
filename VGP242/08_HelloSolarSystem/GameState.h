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
	struct Planet
	{
		float radius;
        float distanceFromSun;
        float orbitSpeed;
		float rotSpeed;
		Engine3D::Graphics::SimpleTextureEffect::RenderData renderData;
	};
	void UpdateCamera(float deltaTime);

	float orbitSpeedChange;
    float rotSpeedChange;
    
    // Planet orbit angle tracking
    float mMercuryAngle = 0.0f;
    float mVenusAngle = 0.0f;
    float mEarthAngle = 0.0f;
    float mMoonAngle = 0.0f;   // Moon orbits around Earth
    float mMarsAngle = 0.0f;
    float mJupiterAngle = 0.0f;
    float mSaturnAngle = 0.0f;
    float mUranusAngle = 0.0f;
    float mNeptuneAngle = 0.0f;
    float mPlutoAngle = 0.0f;

	Engine3D::Graphics::Camera mCamera;
	Engine3D::Graphics::Camera mRenderTargetCamera;
	// GPU communication
	Engine3D::Graphics::SimpleTextureEffect mSimpleTextureEffect;

    //render objects
	Planet mSun;
	Planet mEarth;
	Planet mMoon;
    Planet mMercury;
    Planet mVenus;
    Planet mMars;
    Planet mJupiter;
    Planet mSaturn;
    Planet mUranus;
    Planet mNeptune;
    Planet mPluto;
    Planet mSkySphere;
	//render target
    Engine3D::Graphics::RenderTarget mRenderTarget;
};