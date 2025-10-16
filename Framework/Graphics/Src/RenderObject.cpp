#include "Precompiled.h"
#include "RenderObject.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;

void RenderObject::Terminate()
{
	meshBuffer.Terminate();
	TextureManager* tm = TextureManager::Get();
	tm->ReleaseTexture(diffuseMapId);
	tm->ReleaseTexture(specMapId);
	tm->ReleaseTexture(normalMapId);
	tm->ReleaseTexture(bumpMapId);
}