#pragma once

#include "Component.h"

namespace Engine3D
{
    class UIComponent : public Component
    {
    public:
        virtual void Render() = 0;
    };
}