#pragma once

#include "Event.h"

namespace Engine3D::Core
{
    using EventListenerId = std::size_t;
    using EventCallBack = std::function<void(const Event&)>;

    class EventManager final
    {
    public:
        static void StaticInitialize();
        static void StaticTerminate();
        static EventManager* Get();
        static void Broadcast(const Event& e);

        EventManager() = default;
        ~EventManager();

        void Initialize();
        void Terminate();

        EventListenerId AddListener(EventTypeId eventId, const EventCallBack& cb);
        void RemoveListener(EventTypeId eventId, EventListenerId listenerId);
    private:
        void BroadcastPrivate(const Event& e);

        using EventListeners = std::unordered_map<EventTypeId, std::unordered_map<EventListenerId, EventCallBack>>;
        EventListeners mEventListeners;
        EventListenerId mNextListenerId = 0;

    };
}