#pragma once

#include "CustomTypeIds.h"
#include <3DEngine/Inc/3DEngine.h>

class CustomDebugDrawComponent;

class CustomDebugDrawService : public Engine3D::Service
{
public:
    SET_TYPE_ID(CustomServiceId::CustomDebugDrawDisplay);

    void Render() override;

    void Register(const CustomDebugDrawComponent* debugDrawComponent);
    void Unregister(const CustomDebugDrawComponent* debugDrawComponent);

private:
    using CustomDebugDrawComponents = std::vector<const CustomDebugDrawComponent*>;
    CustomDebugDrawComponents mCustomDebugDrawComponents;
};