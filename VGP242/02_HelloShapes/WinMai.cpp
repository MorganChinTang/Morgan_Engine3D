#include <3DEngine/Inc/3DEngine.h>
#include "ShapeState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int) 
{
    Engine3D::AppConfig config;
    config.appName = L"Hello Shapes";

    Engine3D::App& myApp = Engine3D::MainApp();
    myApp.AddState<ShapeState>("ShapeState");
    myApp.AddState<TriangleShapeState>("TriangleShapeState");
    myApp.AddState<TreeShapeState>("TreeShapeState");
    myApp.AddState<DiamondShapeState>("DiamondShapeState");


    myApp.Run(config);

    return 0;
}