#pragma once

#include "CustomTypeIds.h"
#include <3DEngine/Inc/3DEngine.h>

class CustomDebugDrawComponent : public Engine3D::Component
{
public:

    SET_TYPE_ID(CustomComponentId::CustomDebugDraw);

    void Initialize() override;
    void Terminate() override;
    void DebugUI() override;
    void Deserialize(const rapidjson::Value& value) override;

    void AddDebugDraw() const;

private:
    const Engine3D::TransformComponent* mTransformComponent = nullptr;
    Engine3D::Math::Vector3 mPosition = Engine3D::Math::Vector3::Zero;
    Engine3D::Graphics::Color mColor = Engine3D::Graphics::Colors::White;

    uint32_t mSlices = 0;
    uint32_t mRings = 0;
    float mRadius = 0;
};