#pragma once

#include <3DEngine/Inc/3DEngine.h>

enum class GameEventType
{
    PlaySequence = 1,
    SceneSelected
};

class PlaySequenceEvent : public Engine3D::Core::Event
{
public:
    PlaySequenceEvent() = default;
    SET_EVENT_TYPE_ID(GameEventType::PlaySequence)
};

class SceneSelectedEvent : public Engine3D::Core::Event
{
public:
    explicit SceneSelectedEvent(int index)
        : sceneIndex(index)
    {
    }

    SET_EVENT_TYPE_ID(GameEventType::SceneSelected)

    int sceneIndex = 0;
};
