#pragma once

namespace Engine3D
{
    enum class ComponentId
    {
        Invalid,    //default value, invalid id
        Transform,
        Camera,
        FPSCamera,
        Mesh,
        Model,
        Animator,
        RigidBody,
        SoundEffect,               // adds a sound effect to an object
        SoundBank,                 // adds a group of sounds to an object
        UIText,                    // adds a UI text Component
        UIBSprite,                 // adds a UI BSprite Component
        UButton,
        Count       // last value, can be used to chain custom component

    };

    enum class ServiceId
    {
        Invalid,
        Camera,
        Render,
        Physics,
        UIRender,
        Count
    };
}

#define SET_TYPE_ID(id)\
static uint32_t StaticGetTypeId(){ return static_cast<uint32_t>(id);}\
uint32_t GetTypeId() const override { return StaticGetTypeId(); }
