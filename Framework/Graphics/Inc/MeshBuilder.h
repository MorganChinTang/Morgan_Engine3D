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
        static MeshPX CreateCubePX(float size);

        //hw:Create the shapes and make if keyPressed switch between the shapes
        //pyramid
        static MeshPC CreatePyramidPC(float size);

        //rectangle
        static MeshPC CreateRectanglePC(float width, float height, float depth);

        //plane
        static MeshPC CreatePlanePC(int numRows, int numColumns, float spacing, bool horizontal);
        static MeshPX CreatePlanePX(int numRows, int numColumns, float spacing, bool horizontal);
        static Mesh CreatePlane(int numRows, int numColumns, float spacing, bool horizontal);

        //cylinder
        static MeshPC CreateCylinderPC(int slices, int rings);

        //sphere
        static MeshPC CreateSpherePC(int slices, int rings, float radius);
        static MeshPX CreateSpherePX(int slices, int rings, float radius);
        static Mesh CreateSphere(int slices, int rings, float radius);


        //sky
        static MeshPX CreateSkySpherePX(int slices, int rings, int radius);

        //load obj
        static MeshPX CreateOBJPX(const std::filesystem::path& filePath, float scale);

    };
}