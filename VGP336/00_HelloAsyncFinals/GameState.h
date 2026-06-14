#pragma once

#include "AsyncLoadController.h"
#include "GridPlacementController.h"
#include <3DEngine/Inc/3DEngine.h>

class GameState : public Engine3D::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;

private:
    std::filesystem::path mLevelFile;
    Engine3D::GameWorld mGameWorld;
    GridPlacementController mGridPlacementController;
    AsyncLoadController mAsyncLoadController;

};