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

    void CreatePlaneIndices(std::vector<uint32_t>& indices, int numRows, int numColumns)
    {
        for (int r = 0; r < numRows; ++r)
        {
            for (int c = 0; c < numColumns; ++c)
            {
                int i = (r * (numColumns + 1)) + c;

                //triangle 0
                indices.push_back(i);
                indices.push_back(i + numColumns + 1);
                indices.push_back(i + numColumns + 2);
                //triangle 1
                indices.push_back(i);
                indices.push_back(i + numColumns + 2);
                indices.push_back(i + 1);
            }
        }
    }

    void CreateCapIndices(std::vector<uint32_t>& indices, int slices, int topIndex, int bottomIndex)
    {
        for (int s = 0; s < slices; ++s)
        {
            //bottom triangle
            indices.push_back(bottomIndex);
            indices.push_back(s);
            indices.push_back(s + 1);
            //top triangle
            int topRowIndex = topIndex - slices - 1 + s;
            indices.push_back(topIndex);
            indices.push_back(topRowIndex + 1);
            indices.push_back(topRowIndex);
        }
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
        //1, 2, 3, 1, 3, 4,
        // Sides
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        // Bottom face
        1, 4, 3,
        1, 3, 2
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
        0, 1, 2,
        0, 2, 3,
        // back face
        7, 6, 5,
        7, 5, 4,
        // right face
        3, 2, 6,
        3, 6, 7,
        // left face
        4, 5, 1,
        4, 1, 0,
        // top face
        1, 5, 6,
        1, 6, 2,
        // bottom face
        0, 3, 7,
        0, 7, 4
    };

    return mesh;
}

MeshPC Engine3D::Graphics::MeshBuilder::CreatePlanePC(int numRows, int numColumns, float spacing, bool horizontal)
{
    MeshPC mesh;
    int index = rand() % 100;
    const float hpw = static_cast<float>(numColumns) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;

    float w = -hpw;
    float h = -hph;
    for (int r = 0; r <= numRows; r++)
    {
        for(int c = 0; c <= numColumns; c++)
        {
            Math::Vector3 pos = (horizontal) ? Math::Vector3(w, 0.0f, h) : Math::Vector3(w, h, 0.0f);
            mesh.vertices.push_back({ pos, GetNextColor(index) });
            w += spacing;
        }
        w = -hpw;
        h += spacing;
    }

    CreatePlaneIndices(mesh.indices, numRows, numColumns);

    return mesh;
}

MeshPX Engine3D::Graphics::MeshBuilder::CreatePlanePX(int numRows, int numColumns, float spacing, bool horizontal)
{
    MeshPX mesh;

    int index = rand() % 100;
    const float hpw = static_cast<float>(numColumns) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;
    const float uInc = 1.0f / static_cast<float>(numColumns);
    const float vInc = -1.0f / static_cast<float>(numRows);

    float w = -hpw;
    float h = -hph;
    float u = 0.0f;
    float v = 1.0f;


    for (int r = 0; r <= numRows; r++)
    {
        for (int c = 0; c <= numColumns; c++)
        {
            Math::Vector3 pos = (horizontal) ? Math::Vector3(w, 0.0f, h) : Math::Vector3(w, h, 0.0f);
            mesh.vertices.push_back({ pos, {u,v } });
            w += spacing;
            u += uInc;
        }
        w = -hpw;
        h += spacing;
        u = 0.0f;
        v += vInc;
    }

    CreatePlaneIndices(mesh.indices, numRows, numColumns);

    return mesh;
}

Mesh Engine3D::Graphics::MeshBuilder::CreatePlane(int numRows, int numColumns, float spacing, bool horizontal)
{
    Mesh mesh;

    int index = rand() % 100;
    const float hpw = static_cast<float>(numColumns) * spacing * 0.5f;
    const float hph = static_cast<float>(numRows) * spacing * 0.5f;
    const float uInc = 1.0f / static_cast<float>(numColumns);
    const float vInc = -1.0f / static_cast<float>(numRows);

    float w = -hpw;
    float h = -hph;
    float u = 0.0f;
    float v = 1.0f;

    Math::Vector3 norm = (horizontal) ? Math::Vector3::YAxis : -Math::Vector3::ZAxis;
    Math::Vector3 tan = Math::Vector3::XAxis;

    for (int r = 0; r <= numRows; r++)
    {
        for (int c = 0; c <= numColumns; c++)
        {
            Math::Vector3 pos = (horizontal) ? Math::Vector3(w, 0.0f, h) : Math::Vector3(w, h, 0.0f);
            mesh.vertices.push_back({ pos, norm, tan, {u,v } });
            w += spacing;
            u += uInc;
        }
        w = -hpw;
        h += spacing;
        u = 0.0f;
        v += vInc;
    }

    CreatePlaneIndices(mesh.indices, numRows, numColumns);

    return mesh;
}

MeshPC Engine3D::Graphics::MeshBuilder::CreateCylinderPC(int slices, int rings)
{
    MeshPC mesh;
    int index = rand() % 100;

    const float hh = static_cast<float>(rings) * 0.5f;
    const float fSlices = static_cast<float>(slices);

    for (int r = 0; r <= rings; r++)
    {
        float ring = static_cast<float>(r);
        for (int s = 0; s <= slices; s++)
        {
            float slice = static_cast<float>(s);
            float rotation = (slice / fSlices) * Math::Constants::TwoPi;

            mesh.vertices.push_back({ {
                    sin(rotation),
                    ring - hh,
                    -cos(rotation)},
                GetNextColor(index) });
        }
    }

    mesh.vertices.push_back({ {0.0f, hh, 0.0f}, GetNextColor(index) });
    mesh.vertices.push_back({ {0.0f, -hh, 0.0f}, GetNextColor(index) });

    CreatePlaneIndices(mesh.indices, rings, slices);
    CreateCapIndices(mesh.indices, slices, mesh.vertices.size() - 2, mesh.vertices.size() - 1);

    return mesh;
}

MeshPC Engine3D::Graphics::MeshBuilder::CreateSpherePC(int slices, int rings, float radius)
{
    MeshPC mesh;
    int index = rand() % 100;

    float vertRotation = Math::Constants::Pi / static_cast<float>(rings);
    float horRotation = Math::Constants::TwoPi / static_cast<float>(slices);

    for (int r = 0; r <= rings; r++)
    {
        float ring = static_cast<float>(r);
        float phi = ring * vertRotation;
        for (int s = 0; s <= slices; s++)
        {
            float slice = static_cast<float>(s);
            float rotation = slice * horRotation;
            mesh.vertices.push_back({ {
                    radius * sin(rotation) * sin(phi),
                    radius* cos(phi),
                    radius* cos(rotation)* sin(phi)},
                    GetNextColor(index) });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

MeshPX Engine3D::Graphics::MeshBuilder::CreateSpherePX(int slices, int rings, float radius)
{
    MeshPX mesh;

    float vertRotation = Math::Constants::Pi / static_cast<float>(rings);
    float horRotation = Math::Constants::TwoPi / static_cast<float>(slices);

    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);

    for (int r = 0; r <= rings; r++)
    {
        float ring = static_cast<float>(r);
        float phi = ring * vertRotation;
        for (int s = 0; s <= slices; s++)
        {
            float slice = static_cast<float>(s);
            float rotation = slice * horRotation;

            float u = uStep * slice;
            float v = vStep * ring;
            mesh.vertices.push_back({ {
                    radius * sin(rotation) * sin(phi),
                    radius * cos(phi),
                    radius * cos(rotation) * sin(phi)},
                    { u, v }});
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

Mesh Engine3D::Graphics::MeshBuilder::CreateSphere(int slices, int rings, float radius)
{
    Mesh mesh;

    float vertRotation = Math::Constants::Pi / static_cast<float>(rings);
    float horRotation = Math::Constants::TwoPi / static_cast<float>(slices);

    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);

    for (int r = 0; r <= rings; r++)
    {
        float ring = static_cast<float>(r);
        float phi = ring * vertRotation;
        for (int s = 0; s <= slices; s++)
        {
            float slice = static_cast<float>(s);
            float rotation = slice * horRotation;

            float u = uStep * slice;
            float v = vStep * ring;

            Math::Vector3 pos = {
                    radius * sin(rotation) * sin(phi),
                    radius * cos(phi),
                    radius * cos(rotation) * sin(phi)};
            Math::Vector3 norm = Math::Normalize(pos);
            Math::Vector3 tan = abs(Math::Dot(norm, Math::Vector3::YAxis)) < 0.999f ?
                Math::Normalize({ -pos.z,0.0f, pos.x }) : Math::Vector3::XAxis;

            mesh.vertices.push_back({pos, norm, tan,{ u, v } });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}

MeshPX Engine3D::Graphics::MeshBuilder::CreateSkySpherePX(int slices, int rings, int radius)
{
    MeshPX mesh;

    float vertRotation = Math::Constants::Pi / static_cast<float>(rings);
    float horRotation = Math::Constants::TwoPi / static_cast<float>(slices);

    float uStep = 1.0f / static_cast<float>(slices);
    float vStep = 1.0f / static_cast<float>(rings);

    for (int r = 0; r <= rings; r++)
    {
        float ring = static_cast<float>(r);
        float phi = ring * vertRotation;
        for (int s = 0; s <= slices; s++)
        {
            float slice = static_cast<float>(s);
            float rotation = slice * horRotation;

            float u = uStep * slice;
            float v = vStep * ring;
            mesh.vertices.push_back({ {
                    radius * cos(rotation) * sin(phi),
                    radius * cos(phi),
                    radius * sin(rotation) * sin(phi)},
                    { u, v } });
        }
    }

    CreatePlaneIndices(mesh.indices, rings, slices);

    return mesh;
}


MeshPC MeshBuilder::CreateCubePC(float size)
{
    MeshPC mesh;

    int index = rand() % 100;
    const float hs = 0.5f;

    // Define vertices for the cube
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

    // Define indices for the cube
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

MeshPX Engine3D::Graphics::MeshBuilder::CreateCubePX(float size)
{
    return MeshPX();
}

MeshPX Engine3D::Graphics::MeshBuilder::CreateOBJPX(const std::filesystem::path& filePath, float scale)
{
    MeshPX mesh;
    FILE* file = nullptr;
    fopen_s(&file, filePath.u8string().c_str(), "r");
    ASSERT(file != nullptr, "MeshBuilder: Cant open file %s", filePath.u8string().c_str());

    //read in file
    std::vector<Math::Vector3> positions;
    std::vector<Math::Vector2> uvCoords;
    std::vector<uint32_t> positionIndices;
    std::vector<uint32_t> uvIndices;

    while (true)
    {
        char buffer[128];
        int result = fscanf_s(file, "%s", buffer, (uint32_t)std::size(buffer));
        if(result ==EOF)
            break;
        if (strcmp(buffer, "v") == 0)
        {
            float x, y, z = 0;
            fscanf_s(file, "%f %f %f\n", &x, &y, &z);
            positions.push_back({ x, y, z });
        }
        else if (strcmp(buffer, "vt") == 0)
        {
            float u, v = 0;
            fscanf_s(file, "%f %f\n", &u, &v);
            uvCoords.push_back({ u,1.0f - v });
        }
        else if (strcmp(buffer, "f") == 0)
        {
            uint32_t p[4];
            uint32_t uv[4];
            int count = fscanf_s(file, "%d/%d/%*d %d/%d/%*d %d/%d/%*d %d/%d/%*d\n", &p[0], &uv[0], &p[1], &uv[1], &p[2], &uv[2], &p[3], &uv[3]);
            ASSERT(count == 6, "MeshBuilder: Format not recognize for %s", filePath.u8string().c_str());
            if (count % 3 == 0)
            {
                for (uint32_t i = 0; i < 3; ++i)
                {
                    positionIndices.push_back(p[i]);
                    uvIndices.push_back(uv[i]);
                }
            }
            else
            {
                //triangle 1
                positionIndices.push_back(p[0] - 1);
                positionIndices.push_back(p[1] - 1);
                positionIndices.push_back(p[2] - 1);
                //triangle 2
                positionIndices.push_back(p[0] - 1);
                positionIndices.push_back(p[2] - 1);
                positionIndices.push_back(p[3] - 1);

                //triangle 1
                uvIndices.push_back(uv[0] - 1);
                uvIndices.push_back(uv[1] - 1);
                uvIndices.push_back(uv[2] - 1);
                //triangle 2
                uvIndices.push_back(uv[0] - 1);
                uvIndices.push_back(uv[2] - 1);
                uvIndices.push_back(uv[3] - 1);
            }
        }
    }
    fclose(file);
    mesh.vertices.resize(positions.size());
    for (uint32_t i = 0; i < positions.size(); ++i)
    {
        mesh.vertices[i].position = positions[i] * scale;
    }
    if (uvCoords.size() > 0)
    {
        for (uint32_t i = 0; i < uvIndices.size(); ++i)
        {
            mesh.vertices[positionIndices[i]].uvCoord = uvCoords[uvIndices[i]];
        }
    }
    
    mesh.indices = std::move(positionIndices);

    return mesh;
}