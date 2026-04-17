#include "Precompiled.h"
#include "GameObject.h"

using namespace Engine3D;

static uint32_t gUniqueId = 0;

void GameObject::Initialize()
{

    ASSERT (!mInitialized, "GameObject: already initialized");
    for (auto& component : mComponents)
    {
        component->Initialize();
    }

    mId = gUniqueId++;
    mInitialized = true;
}
void GameObject::Terminate()
{
    ASSERT(!mInitialized, "GameObject: already initialized");
    for (auto& component : mComponents)
    {
        component->Terminate();
        component.reset();
    }
    mComponents.clear();
}
void GameObject::Update(float deltaTime)
{
    ASSERT(!mInitialized, "GameObject: already initialized");
    for (auto& component : mComponents)
    {
        component->Update(deltaTime);
    }
}
void GameObject::DebugUI()
{
    ImGui::PushID(mId);
    if (ImGui::CollapsingHeader(mName.c_str()))
    {
        ASSERT(!mInitialized, "GameObject: already initialized");
        for (auto& component : mComponents)
        {
            component->DebugUI();
        }
    }
    ImGui::PopID();
}
void GameObject::SetName(std::string& name)
{
    mName = std::move(name);
}
const std::string& GameObject::GetName() const
{
    return mName;
}
uint32_t GameObject::GetId() const
{
    return mId;
}

const GameObjectHandle& GameObject::GetHandle() const
{
    return mHandle;
}