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

MeshPC Engine3D::Graphics::MeshBuilder::CreatePyramd(float size)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hs = size * 0.5f;

    return mesh;
}

MeshPC Engine3D::Graphics::MeshBuilder::CreateRectangle(float width, float height, float depth)
{
    return MeshPC();
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
