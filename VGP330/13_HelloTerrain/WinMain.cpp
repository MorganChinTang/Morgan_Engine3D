#include <3DEngine/Inc/3DEngine.h>
#include "GameState.h"

using namespace Engine3D;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{
    AppConfig config;
    config.appName = L"Hello Terrain";

    App& myApp = MainApp();
    myApp.AddState<GameState>("GameState");
    myApp.Run(config);

    return 0;
}