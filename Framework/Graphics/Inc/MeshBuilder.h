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

        //hw:Create the shapes and make if keyPressed switch between the shapes
        //pyramid
        static MeshPC CreatePyramidPC(float size);

        //rectangle
        static MeshPC CreateRectanglePC(float width, float height, float depth);
    };
}