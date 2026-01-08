#pragma once

#include "Animation.h"

namespace Engine3D::Graphics
{
    class AnimationBuilder
    {
    public:
        AnimationBuilder& AddPositionKey(const Math::Vector3& position, float time);
        AnimationBuilder& AddRotationKey(const Math::Quaternion& rotation, float time);
        AnimationBuilder& AddScaleKey(const Math::Vector3& scale, float time);

        [[nodiscard]] Animation Build();
    private:
        Animation mWorkingCopy;
    };
}