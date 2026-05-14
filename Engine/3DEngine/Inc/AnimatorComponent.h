#pragma once

#include "Component.h"

namespace Engine3D
{

    class AnimatorComponent : public Component
    {
    public:
        SET_TYPE_ID(ComponentId::Animator);

        void Initialize() override;
        void Update(float deltatime) override;
        void DebugUI() override;

        bool Play(int index, bool looping = false);

        Graphics::Animator& GetAnimator();
        const Graphics::Animator& GetAnimator() const;

    private:
        Graphics::Animator mAnimator;
    };
}