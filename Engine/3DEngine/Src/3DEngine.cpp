#include "Precompiled.h"
#include "3DEngine.h"

Engine3D::App& Engine3D::MainApp()
{
    static App app;
    return app;
}