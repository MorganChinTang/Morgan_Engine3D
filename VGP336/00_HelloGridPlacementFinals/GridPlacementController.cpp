#include "GridPlacementController.h"

#include <algorithm>
#include <cmath>

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Math;

namespace
{
    Vector4 TransformPoint(const Vector4& v, const Matrix4& m)
    {
        return
        {
            v.x * m._11 + v.y * m._21 + v.z * m._31 + v.w * m._41,
            v.x * m._12 + v.y * m._22 + v.z * m._32 + v.w * m._42,
            v.x * m._13 + v.y * m._23 + v.z * m._33 + v.w * m._43,
            v.x * m._14 + v.y * m._24 + v.z * m._34 + v.w * m._44
        };
    }

    float ComputeYaw(const Quaternion& rotation)
    {
        const Matrix4 rotMat = Matrix4::MatrixRotationQuaternion(rotation);
        const Vector3 forward = Normalize(GetLook(rotMat));
        return atan2f(forward.x, forward.z);
    }
}

void GridPlacementController::Initialize(GameWorld& gameWorld)
{
    mGameWorld = &gameWorld;
    mPickableFurniture.clear();
    mPreviousMouseWheel = InputSystem::Get()->GetMouseMoveZ();

    CameraService* cameraService = mGameWorld->GetService<CameraService>();
    if (cameraService != nullptr)
    {
        Camera& camera = const_cast<Camera&>(cameraService->GetMain());
        camera.SetDirection(Normalize(Vector3(0.0f, -1.0f, 0.35f)));
    }
}

void GridPlacementController::Terminate()
{
    mHeldState = {};
    mPickableFurniture.clear();
    mLockedPlacements.clear();
    mGameWorld = nullptr;
}

void GridPlacementController::RegisterPickableFurniture(GameObject& gameObject)
{
    auto iter = std::find(mPickableFurniture.begin(), mPickableFurniture.end(), &gameObject);
    if (iter == mPickableFurniture.end())
    {
        mPickableFurniture.push_back(&gameObject);
    }
}

void GridPlacementController::Update(float deltaTime)
{
    if (mGameWorld == nullptr)
    {
        return;
    }

    UpdateCamera(deltaTime);
    ApplyLockedPlacements();

    Vector3 mouseGroundPoint = Vector3::Zero;
    const bool hasGroundHit = GetMouseGroundPoint(mouseGroundPoint);

    InputSystem* input = InputSystem::Get();
    if (mHeldState.gameObject == nullptr)
    {
        if (hasGroundHit && !ImGui::GetIO().WantCaptureMouse && input->IsMousePressed(MouseButton::LBUTTON))
        {
            TryPickObject(mouseGroundPoint);
        }
        return;
    }

    if (!ImGui::GetIO().WantCaptureKeyboard)
    {
        if (input->IsKeyPressed(KeyCode::Q))
        {
            --mHeldState.rotationSteps;
        }
        if (input->IsKeyPressed(KeyCode::E))
        {
            ++mHeldState.rotationSteps;
        }
    }

    if (hasGroundHit)
    {
        UpdateHeldObject(mouseGroundPoint);
    }

    if (ImGui::GetIO().WantCaptureMouse)
    {
        return;
    }

    if (input->IsMousePressed(MouseButton::LBUTTON))
    {
        PlaceHeldObject();
    }
    else if (input->IsMousePressed(MouseButton::RBUTTON))
    {
        CancelHeldObject();
    }
}

void GridPlacementController::DebugUI()
{
    if (!ImGui::CollapsingHeader("GridPlacement", ImGuiTreeNodeFlags_DefaultOpen))
    {
        return;
    }

    ImGui::DragFloat("GridSize", &mGridSize, 0.05f, 0.1f, 10.0f);
    ImGui::DragFloat("GroundHeight", &mGroundHeight, 0.05f, -20.0f, 20.0f);
    ImGui::DragFloat("PickRadius", &mPickRadius, 0.05f, 0.1f, 10.0f);

    ImGui::Separator();
    ImGui::DragFloat("PanSpeed", &mPanSpeed, 0.1f, 0.1f, 100.0f);
    ImGui::DragFloat("ZoomSpeed", &mZoomSpeed, 0.1f, 0.1f, 100.0f);
    ImGui::DragFloat("MinCamHeight", &mMinCameraHeight, 0.1f, 0.5f, 200.0f);
    ImGui::DragFloat("MaxCamHeight", &mMaxCameraHeight, 0.1f, 1.0f, 500.0f);

    ImGui::Separator();
    if (mHeldState.gameObject != nullptr)
    {
        ImGui::Text("Holding: %s", mHeldState.gameObject->GetName().c_str());
    }
    else
    {
        ImGui::Text("Holding: None");
    }
}

void GridPlacementController::UpdateCamera(float deltaTime)
{
    if (mGameWorld == nullptr)
    {
        return;
    }

    CameraService* cameraService = mGameWorld->GetService<CameraService>();
    if (cameraService == nullptr)
    {
        return;
    }

    Camera& camera = const_cast<Camera&>(cameraService->GetMain());
    InputSystem* input = InputSystem::Get();

    camera.SetDirection(Normalize(Vector3(0.0f, -1.0f, 0.35f)));

    if (!ImGui::GetIO().WantCaptureKeyboard)
    {
        Vector3 forward = camera.GetDirection();
        forward.y = 0.0f;
        if (MagnitudeSqr(forward) < 0.0001f)
        {
            forward = Vector3::ZAxis;
        }
        else
        {
            forward = Normalize(forward);
        }

        const Vector3 right = Normalize(Cross(Vector3::YAxis, forward));

        Vector3 movement = Vector3::Zero;
        if (input->IsKeyDown(KeyCode::W))
        {
            movement += forward;
        }
        if (input->IsKeyDown(KeyCode::S))
        {
            movement -= forward;
        }
        if (input->IsKeyDown(KeyCode::D))
        {
            movement += right;
        }
        if (input->IsKeyDown(KeyCode::A))
        {
            movement -= right;
        }

        if (MagnitudeSqr(movement) > 0.0f)
        {
            movement = Normalize(movement);
            camera.SetPosition(camera.GetPosition() + movement * (mPanSpeed * deltaTime));
        }
    }

    const float currentWheel = input->GetMouseMoveZ();
    const float wheelDelta = currentWheel - mPreviousMouseWheel;
    mPreviousMouseWheel = currentWheel;

    if (!ImGui::GetIO().WantCaptureMouse && Abs(wheelDelta) > 0.0001f)
    {
        Vector3 zoomPosition = camera.GetPosition();
        zoomPosition += camera.GetDirection() * (wheelDelta * mZoomSpeed);
        zoomPosition.y = Clamp(zoomPosition.y, mMinCameraHeight, mMaxCameraHeight);
        camera.SetPosition(zoomPosition);
    }
}

bool GridPlacementController::GetMouseGroundPoint(Vector3& worldPoint) const
{
    if (mGameWorld == nullptr)
    {
        return false;
    }

    const CameraService* cameraService = mGameWorld->GetService<CameraService>();
    if (cameraService == nullptr)
    {
        return false;
    }

    const Camera& camera = cameraService->GetMain();
    const InputSystem* input = InputSystem::Get();

    const float width = static_cast<float>(GraphicsSystem::Get()->GetBackBufferWidth());
    const float height = static_cast<float>(GraphicsSystem::Get()->GetBackBufferHeight());
    if (width <= 0.0f || height <= 0.0f)
    {
        return false;
    }

    const float mouseX = static_cast<float>(input->GetMouseScreenX());
    const float mouseY = static_cast<float>(input->GetMouseScreenY());

    const float ndcX = (2.0f * mouseX / width) - 1.0f;
    const float ndcY = 1.0f - (2.0f * mouseY / height);

    const Vector4 nearClip(ndcX, ndcY, 0.0f, 1.0f);
    const Vector4 farClip(ndcX, ndcY, 1.0f, 1.0f);

    const Matrix4 viewProj = camera.GetViewMatrix() * camera.GetProjectionMatrix();
    const Matrix4 invViewProj = Inverse(viewProj);

    Vector4 nearH = TransformPoint(nearClip, invViewProj);
    Vector4 farH = TransformPoint(farClip, invViewProj);

    if (Abs(nearH.w) < 0.0001f || Abs(farH.w) < 0.0001f)
    {
        return false;
    }

    const Vector3 nearWorld(nearH.x / nearH.w, nearH.y / nearH.w, nearH.z / nearH.w);
    const Vector3 farWorld(farH.x / farH.w, farH.y / farH.w, farH.z / farH.w);

    const Vector3 rayDir = Normalize(farWorld - nearWorld);
    if (Abs(rayDir.y) < 0.0001f)
    {
        return false;
    }

    const float t = (mGroundHeight - nearWorld.y) / rayDir.y;
    if (t < 0.0f)
    {
        return false;
    }

    worldPoint = nearWorld + rayDir * t;
    return true;
}

void GridPlacementController::ApplyLockedPlacements()
{
    for (auto& [gameObject, transform] : mLockedPlacements)
    {
        if (gameObject == nullptr)
        {
            continue;
        }

        TransformComponent* transformComponent = gameObject->GetComponent<TransformComponent>();
        if (transformComponent == nullptr)
        {
            continue;
        }

        transformComponent->position = transform.position;
        transformComponent->rotation = transform.rotation;
        transformComponent->scale = transform.scale;

        RigidBodyComponent* rigidBody = gameObject->GetComponent<RigidBodyComponent>();
        if (rigidBody != nullptr)
        {
            rigidBody->SetPosition(transform.position);
            rigidBody->SetVelocity(Vector3::Zero);
            rigidBody->SetAngularVelocity(Vector3::Zero);
        }
    }
}

void GridPlacementController::TryPickObject(const Vector3& mouseGroundPoint)
{
    GameObject* closest = nullptr;
    float bestDistSq = mPickRadius * mPickRadius;

    for (GameObject* furniture : mPickableFurniture)
    {
        if (furniture == nullptr)
        {
            continue;
        }

        TransformComponent* transform = furniture->GetComponent<TransformComponent>();
        if (transform == nullptr)
        {
            continue;
        }

        const Vector3 objectPos(transform->position.x, mGroundHeight, transform->position.z);
        const Vector3 targetPos(mouseGroundPoint.x, mGroundHeight, mouseGroundPoint.z);
        const float distSq = DistanceSqr(objectPos, targetPos);
        if (distSq < bestDistSq)
        {
            bestDistSq = distSq;
            closest = furniture;
        }
    }

    if (closest == nullptr)
    {
        return;
    }

    TransformComponent* transformComponent = closest->GetComponent<TransformComponent>();
    if (transformComponent == nullptr)
    {
        return;
    }

    mHeldState.gameObject = closest;
    mHeldState.transformComponent = transformComponent;
    mHeldState.rigidBodyComponent = closest->GetComponent<RigidBodyComponent>();
    mHeldState.originalTransform = { transformComponent->position, transformComponent->rotation, transformComponent->scale };
    mHeldState.baseYaw = ComputeYaw(transformComponent->rotation);
    mHeldState.rotationSteps = 0;

    auto iter = mLockedPlacements.find(mHeldState.gameObject);
    mHeldState.hadLockedPlacement = (iter != mLockedPlacements.end());
    mLockedPlacements.erase(mHeldState.gameObject);

    if (mHeldState.rigidBodyComponent != nullptr)
    {
        mHeldState.rigidBodyComponent->SetVelocity(Vector3::Zero);
        mHeldState.rigidBodyComponent->SetAngularVelocity(Vector3::Zero);
    }
}

void GridPlacementController::UpdateHeldObject(const Vector3& mouseGroundPoint)
{
    if (mHeldState.transformComponent == nullptr)
    {
        return;
    }

    Vector3 snappedPosition = mouseGroundPoint;
    snappedPosition.x = Snap(snappedPosition.x, mGridSize);
    snappedPosition.z = Snap(snappedPosition.z, mGridSize);
    snappedPosition.y = mGroundHeight;

    const float yaw = mHeldState.baseYaw + (static_cast<float>(mHeldState.rotationSteps) * Constants::HalfPi);
    const Quaternion snappedRotation = Quaternion::CreateFromYawPitchRoll(yaw, 0.0f, 0.0f);

    mHeldState.transformComponent->position = snappedPosition;
    mHeldState.transformComponent->rotation = snappedRotation;

    if (mHeldState.rigidBodyComponent != nullptr)
    {
        mHeldState.rigidBodyComponent->SetPosition(snappedPosition);
        mHeldState.rigidBodyComponent->SetVelocity(Vector3::Zero);
        mHeldState.rigidBodyComponent->SetAngularVelocity(Vector3::Zero);
    }
}

void GridPlacementController::PlaceHeldObject()
{
    if (mHeldState.gameObject == nullptr || mHeldState.transformComponent == nullptr)
    {
        mHeldState = {};
        return;
    }

    Transform placedTransform;
    placedTransform.position = mHeldState.transformComponent->position;
    placedTransform.rotation = mHeldState.transformComponent->rotation;
    placedTransform.scale = mHeldState.transformComponent->scale;

    mLockedPlacements[mHeldState.gameObject] = placedTransform;
    mHeldState = {};
}

void GridPlacementController::CancelHeldObject()
{
    if (mHeldState.transformComponent == nullptr)
    {
        mHeldState = {};
        return;
    }

    mHeldState.transformComponent->position = mHeldState.originalTransform.position;
    mHeldState.transformComponent->rotation = mHeldState.originalTransform.rotation;
    mHeldState.transformComponent->scale = mHeldState.originalTransform.scale;

    if (mHeldState.rigidBodyComponent != nullptr)
    {
        mHeldState.rigidBodyComponent->SetPosition(mHeldState.originalTransform.position);
        mHeldState.rigidBodyComponent->SetVelocity(Vector3::Zero);
        mHeldState.rigidBodyComponent->SetAngularVelocity(Vector3::Zero);
    }

    if (mHeldState.hadLockedPlacement)
    {
        mLockedPlacements[mHeldState.gameObject] = mHeldState.originalTransform;
    }

    mHeldState = {};
}

float GridPlacementController::Snap(float value, float increment)
{
    if (increment <= 0.0f)
    {
        return value;
    }

    return std::round(value / increment) * increment;
}
