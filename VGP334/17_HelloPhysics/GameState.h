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

    Engine3D::Graphics::RenderObject mBallObject;
    Engine3D::Physics::CollisionShape mBallShape;
    Engine3D::Physics::RigidBody mBallRigidBody;

    Engine3D::Graphics::RenderObject mGroundObject;
    Engine3D::Physics::CollisionShape mGroundShape;
    Engine3D::Physics::RigidBody mGroundRigidBody;

    Engine3D::Graphics::StandardEffect mStandardEffect;
};