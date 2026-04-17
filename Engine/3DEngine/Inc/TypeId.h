#pragma once

namespace Engine3D
{
    enum class ComponentId
    {
        Invalid,    //default value, invalid id
        Transform,
        Camera,
        FPSCamera,
        Count       // last value, can be used to chain custom component

    };

    enum class ServiceId
    {
        Invalid,
        Camera,
        Count
    };
}

#define SET_TYPE_ID(id)\
static uint32_t StaticGetTypeId(){ return static_cast<uint32_t>(id);}\
uint32_t GetTypeId() const override { return StaticGetTypeId(); }
