#include "CustomDebugDrawService.h"
#include "CustomDebugDrawComponent.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Math;

void CustomDebugDrawService::Render()
{
	for (CustomDebugDrawComponent* component : mCustomDebugDrawComponents)
	{
		component->AddDebugDraw();
	}
}

const std::vector<CustomDebugDrawComponent*>& CustomDebugDrawService::GetRegisteredComponents() const
{
	return mCustomDebugDrawComponents;
}

void CustomDebugDrawService::Register(CustomDebugDrawComponent* debugDrawComponent)
{
	auto iter = std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), debugDrawComponent);
	if (iter == mCustomDebugDrawComponents.end())
	{
		mCustomDebugDrawComponents.push_back(debugDrawComponent);
	}
}
void CustomDebugDrawService::Unregister(CustomDebugDrawComponent* debugDrawComponent)
{
	auto iter = std::find(mCustomDebugDrawComponents.begin(), mCustomDebugDrawComponents.end(), debugDrawComponent);
	if (iter != mCustomDebugDrawComponents.end())
	{
		mCustomDebugDrawComponents.erase(iter);
	}
}