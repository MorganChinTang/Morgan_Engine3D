#include "Precompiled.h"
#include "RenderObject.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
}