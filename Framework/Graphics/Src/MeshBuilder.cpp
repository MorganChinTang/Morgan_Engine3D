#include "Precompiled.h"
#include "MeshBuilder.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;

namespace
{
    //Debug helper
    Color GetNextColor(int& index)
    {
        constexpr Color colorTable[] =
        {
            Colors::LightPink,
            Colors::LightGreen,
            Colors::LightBlue,
            Colors::Yellow,
            Colors::Cyan,
            Colors::Magenta,
            Colors::White,
        };
        index = (index + 1) % std::size(colorTable);
        return colorTable[index];
    }
}
MeshPC MeshBuilder::CreateCubePC(float size, const Color& color)
{
    MeshPC mesh;

    const float hs = 0.5f;

    //front face
    mesh.vertices.push_back({ { -hs, -hs, -hs }, color});
    mesh.vertices.push_back({ { -hs,  hs, -hs }, color});
    mesh.vertices.push_back({ {  hs,  hs, -hs }, color});
    mesh.vertices.push_back({ {  hs, -hs, -hs }, color});

    //back face
    mesh.vertices.push_back({ { -hs, -hs,  hs }, color});
    mesh.vertices.push_back({ { -hs,  hs,  hs }, color});
    mesh.vertices.push_back({ {  hs,  hs,  hs }, color});
    mesh.vertices.push_back({ {  hs, -hs,  hs }, color});

    mesh.indices = {
        //front face
        0,1,2,
        0,2,3,
        //back face
        7,5,4,
        7,6,5,
        //right face
        3,2,6,
        3,6,7,
        //left face
        4,5,1,
        4,1,0,
        //top face
        1,5,6,
        1,6,2,
        //bottom face
        0,3,7,
        0,7,4
    };

    return mesh;
}

MeshPC Engine3D::Graphics::MeshBuilder::CreatePyramidPC(float size)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hs = size * 0.5f;

    // Define vertices for the pyramid
    mesh.vertices.push_back({ { 0.0f,  hs, 0.0f }, GetNextColor(index) }); // Top vertex
    mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index) }); // Base vertices
    mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs,  hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs, -hs,  hs }, GetNextColor(index) });

    // Define indices for the pyramid
    mesh.indices = {
        // Base
        1, 2, 3, 1, 3, 4,
        // Sides
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        // Bottom face
        1, 4, 3, 1, 3, 2
    };

    return mesh;
}

MeshPC Engine3D::Graphics::MeshBuilder::CreateRectanglePC(float width, float height, float depth)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hw = width * 0.5f;
    const float hh = height * 0.5f;
    const float hd = depth * 0.5f;

    // Define vertices for the rectangle
    mesh.vertices.push_back({ { -hw, -hh, -hd }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh, -hd }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh, -hd }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh, -hd }, GetNextColor(index) });

    mesh.vertices.push_back({ { -hw, -hh,  hd }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hw,  hh,  hd }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw,  hh,  hd }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hw, -hh,  hd }, GetNextColor(index) });

    // Define indices for the rectangle
    mesh.indices = {
        // front face
        0, 1, 2, 0, 2, 3,
        // back face
        7, 6, 5, 7, 5, 4,
        // right face
        3, 2, 6, 3, 6, 7,
        // left face
        4, 5, 1, 4, 1, 0,
        // top face
        1, 5, 6, 1, 6, 2,
        // bottom face
        0, 3, 7, 0, 7, 4
    };

    return mesh;
}

MeshPC MeshBuilder::CreateCubePC(float size)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hs = 0.5f;

    //front face
    mesh.vertices.push_back({ { -hs, -hs, -hs }, GetNextColor(index)});
    mesh.vertices.push_back({ { -hs,  hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs, -hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs, -hs }, GetNextColor(index) });

    //back face
    mesh.vertices.push_back({ { -hs, -hs,  hs }, GetNextColor(index) });
    mesh.vertices.push_back({ { -hs,  hs,  hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs,  hs,  hs }, GetNextColor(index) });
    mesh.vertices.push_back({ {  hs, -hs,  hs }, GetNextColor(index) });

    mesh.indices = {
        //front face
        0,1,2,
        0,2,3,
        //back face
        7,5,4,
        7,6,5,
        //right face
        3,2,6,
        3,6,7,
        //left face
        4,5,1,
        4,1,0,
        //top face
        1,5,6,
        1,6,2,
        //bottom face
        0,3,7,
        0,7,4
    };

    return mesh;
}
