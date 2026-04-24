#include "Precompiled.h"
#include "FPSCameraComponent.h"
#include "CameraComponent.h"
#include "GameObject.h"
#include "SaveUtil.h"
using namespace Engine3D;
using namespace Engine3D::Input;

void FPSCameraComponent::Initialize()
{
    mCameraComponent = GetOwner().GetComponent<CameraComponent>();
    ASSERT(mCameraComponent != nullptr, "FPSCameraComponent: no camera component");
}
void FPSCameraComponent::Terminate()
{
    mCameraComponent = nullptr;
}
void FPSCameraComponent::Update(float deltaTime)
{
    Graphics::Camera& camera = mCameraComponent->GetCamera();
    auto input = InputSystem::Get();
    const float moveSpeed = ((input->IsKeyDown(KeyCode::LSHIFT))? mShiftSpeed:mMoveSpeed);
    const float turnSpeed = mTurnSpeed;
    if (input->IsKeyDown(KeyCode::W))
    {
        camera.Walk(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        camera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        camera.Strafe(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        camera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        camera.Rise(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        camera.Rise(moveSpeed * deltaTime);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        camera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        camera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }
}
void FPSCameraComponent::DebugUI()
{
    ImGui::DragFloat("MoveSpeed", &mMoveSpeed, 0.1f, 0.1f, 100.0f);
    ImGui::DragFloat("ShiftSpeed", &mShiftSpeed, 0.1f, 0.1f, 1000.0f);
    ImGui::DragFloat("TurnSpeed", &mTurnSpeed, 0.001f, 0.01f, 100.0f);
}

void FPSCameraComponent::Deserialize(const rapidjson::Value& value)
{
    SaveUtil::ReadFloat("MoveSpeed", mMoveSpeed, value);
    SaveUtil::ReadFloat("ShiftSpeed", mShiftSpeed, value);
    SaveUtil::ReadFloat("TurnSpeed", mTurnSpeed, value);
}
