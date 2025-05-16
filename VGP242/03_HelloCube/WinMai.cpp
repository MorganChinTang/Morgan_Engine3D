#include <3DEngine/Inc/3DEngine.h>
#include "ShapeState.h"

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int) 
{

	Engine3D::AppConfig config;
	config.appName = L"Hello Mesh Builder";

	Engine3D::App& myApp = Engine3D::MainApp();
	myApp.AddState<ShapeState>("ShapeState");
	myApp.Run(config);

	return 0;
}