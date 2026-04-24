#pragma once

#include "Common.h"
//common stays on top
//sort header files in alphabetical order
#include "App.h"
#include "AppState.h"
#include "SaveUtil.h"

// game object info
#include "GameObject.h"
#include "GameWorld.h"
#include "GameObjectHandle.h"
#include "GameObjectFactory.h"

// components
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"

//Services
#include "Service.h"


namespace Engine3D
{
    App& MainApp();
}