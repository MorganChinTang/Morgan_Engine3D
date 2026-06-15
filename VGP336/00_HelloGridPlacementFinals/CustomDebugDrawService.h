#pragma once

#include "CustomTypeIds.h"
#include <3DEngine/Inc/3DEngine.h>

class CustomDebugDrawComponent;

class CustomDebugDrawService : public Engine3D::Service
{
public:
    SET_TYPE_ID(CustomServiceId::CustomDebugDrawDisplay);

    void Render() override;

    const std::vector<CustomDebugDrawComponent*>& GetRegisteredComponents() const;

    void Register(CustomDebugDrawComponent* debugDrawComponent);
    void Unregister(CustomDebugDrawComponent* debugDrawComponent);

private:
    using CustomDebugDrawComponents = std::vector<CustomDebugDrawComponent*>;
    CustomDebugDrawComponents mCustomDebugDrawComponents;
};