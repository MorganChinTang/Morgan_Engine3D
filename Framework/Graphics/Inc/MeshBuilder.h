#pragma once

#include "MeshTypes.h"

namespace Engine3D::Graphics
{
    class MeshBuilder
    {
    public:
        //cube
        static MeshPC CreateCubePC(float size, const Color& color);
        static MeshPC CreateCubePC(float size);

        //hw
        //pyramid
        static MeshPC CreatePyramd(float size);

        //rectangle
        static MeshPC CreateRectangle(float width, float height, float depth);
    };
}