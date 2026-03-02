#include "Precompiled.h"
#include "DebugUtil.h"
#include "EventManager.h"

using namespace Engine3D;
using namespace Engine3D::Core;

namespace
{
    std::unique_ptr<EventManager> sEventManager;
}

void EventManager::StaticInitialize()
{
    ASSERT(sEventManager == nullptr, "EventManager: is already initialized");
    sEventManager = std::make_unique<EventManager>();
    sEventManager->Initialize();
}

void EventManager::StaticTerminate()
{
    if (sEventManager != nullptr)
    {
        sEventManager->Terminate();
        sEventManager.reset();
    }
}

EventManager* EventManager::Get()
{
    ASSERT(sEventManager != nullptr, "EventManager: is not initialized");
    return sEventManager.get();
}

void EventManager::Broadcast(const Event& e)
{
    sEventManager->BroadcastPrivate(e);
}

EventManager::~EventManager()
{
    ASSERT(mEventListeners.empty(), "EventManager: terminate mus be called");
}

void EventManager::Initialize()
{
    mEventListeners.clear();
}

void EventManager::Terminate() {
    mEventListeners.clear();
}

EventListenerId EventManager::AddListener(EventTypeId eventId, const EventCallBack& cb)
{
    ++mNextListenerId;
    mEventListeners[eventId][mNextListenerId] = cb;
    return mNextListenerId;
}

void EventManager::RemoveListener(EventTypeId eventId, EventListenerId listenerId)
{
    auto eventGroupListeners = mEventListeners.find(eventId);
    if (eventGroupListeners != mEventListeners.end())
    {
        auto listener = eventGroupListeners->second.find(listenerId);
        if (listener != eventGroupListeners->second.end())
        {
            eventGroupListeners->second.erase(listener);
        }
    }
}

void EventManager::BroadcastPrivate(const Event& e) {
    auto eventGroupListeners = mEventListeners.find(e.GetTypedId());
    if (eventGroupListeners != mEventListeners.end())
    {
        for (auto& cb : eventGroupListeners->second)
        {
            cb.second(e);
        }
    }
}