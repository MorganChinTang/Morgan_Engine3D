#pragma once

namespace Engine3D::Core
{
    using EventTypeId = std::size_t;

    class Event
    {
    public:
        Event() = default;
        virtual ~Event() = default;

        virtual EventTypeId GetTypedId() const = 0;
    };
}

#define SET_EVENT_TYPE_ID(id) \
    static Engine3D::Core::EventTypeId StaticGetTypeId() { return static_cast<Engine3D::Core::EventTypeId>(id); }\
    Engine3D::Core::EventTypeId GetTypedId() const override { return StaticGetTypeId(); }