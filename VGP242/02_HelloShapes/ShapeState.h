#pragma once

#include <3DEngine/Inc/3DEngine.h>

class ShapeState : public Engine3D::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;

protected:
    virtual void CreateShape();

    struct Vertex
    {
        Engine3D::Math::Vector3 position;
        Engine3D::Graphics::Color color;
    };
    using Vertices = std::vector<Vertex>;
    Vertices mVertices;

    ID3D11Buffer* mVertexBuffer = nullptr;
    ID3D11VertexShader* mVertexShader = nullptr;
    ID3D11InputLayout* mInputLayout = nullptr;
    ID3D11PixelShader* mPixelShader = nullptr;
};

class TriangleShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;
protected:
    void CreateShape() override;
};

class TreeShapeState : public ShapeState
{
public:
    void Update(float deltaTime) override;
protected:
    void CreateShape() override;
};