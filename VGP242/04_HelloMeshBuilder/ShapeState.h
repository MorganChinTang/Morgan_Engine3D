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

    Engine3D::Graphics::MeshPX mMesh;
    Engine3D::Graphics::Camera mCamera;
    Engine3D::Graphics::ConstantBuffer mTransformBuffer;
    Engine3D::Graphics::MeshBuffer mMeshBuffer;
    Engine3D::Graphics::VertexShader mVertexShader;
    Engine3D::Graphics::PixelShader mPixelShader;

    //HelloMeshBuilder
    Engine3D::Graphics::Texture mTexture;
    Engine3D::Graphics::Sampler mSampler;
};
