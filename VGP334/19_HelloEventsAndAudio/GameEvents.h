#pragma once

#include <3DEngine/Inc/3DEngine.h>

enum class GameEventType
{
    PressSpace = 1,
    PressEnter
};

class PressSpaceEvent : public Engine3D::Core::Event
{
public:
    PressSpaceEvent() {}
    SET_EVENT_TYPE_ID(GameEventType::PressSpace)
};

class PressEnterEvent : public Engine3D::Core::Event
{
public:
    PressEnterEvent() {}
    SET_EVENT_TYPE_ID(GameEventType::PressEnter)
};