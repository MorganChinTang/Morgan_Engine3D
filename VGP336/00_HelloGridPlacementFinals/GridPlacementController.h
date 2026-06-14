#pragma once

#include <3DEngine/Inc/3DEngine.h>

#include <vector>
#include <unordered_map>

class GridPlacementController final
{
public:
    void Initialize(Engine3D::GameWorld& gameWorld);
    void Terminate();
    void RegisterPickableFurniture(Engine3D::GameObject& gameObject);

    void Update(float deltaTime);
    void DebugUI();

private:
    using Transform = Engine3D::Graphics::Transform;

    struct HeldState
    {
        Engine3D::GameObject* gameObject = nullptr;
        Engine3D::TransformComponent* transformComponent = nullptr;
        Engine3D::RigidBodyComponent* rigidBodyComponent = nullptr;

        Transform originalTransform;
        float baseYaw = 0.0f;
        int rotationSteps = 0;
        bool hadLockedPlacement = false;
    };

    void UpdateCamera(float deltaTime);
    bool GetMouseGroundPoint(Engine3D::Math::Vector3& worldPoint) const;

    void ApplyLockedPlacements();
    void TryPickObject(const Engine3D::Math::Vector3& mouseGroundPoint);
    void UpdateHeldObject(const Engine3D::Math::Vector3& mouseGroundPoint);
    void PlaceHeldObject();
    void CancelHeldObject();

    static float Snap(float value, float increment);

    Engine3D::GameWorld* mGameWorld = nullptr;
    std::vector<Engine3D::GameObject*> mPickableFurniture;

    std::unordered_map<Engine3D::GameObject*, Transform> mLockedPlacements;
    HeldState mHeldState;

    float mGridSize = 1.0f;
    float mGroundHeight = 0.0f;
    float mPickRadius = 1.75f;

    float mPanSpeed = 8.0f;
    float mZoomSpeed = 3.0f;
    float mMinCameraHeight = 4.0f;
    float mMaxCameraHeight = 45.0f;
    float mPreviousMouseWheel = 0.0f;
};
