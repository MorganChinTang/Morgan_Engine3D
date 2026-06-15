#pragma once

#include <3DEngine/Inc/TypeId.h>

enum class CustomComponentId
{
	CustomDebugDraw = static_cast<int>(Engine3D::ComponentId::Count)

};

enum class CustomServiceId
{
	CustomDebugDrawDisplay = static_cast<int>(Engine3D::ServiceId::Count)
};