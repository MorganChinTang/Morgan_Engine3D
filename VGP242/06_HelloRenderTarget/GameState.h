#pragma once

#include <3DEngine/Inc/3DEngine.h>

class GameState : public Engine3D::AppState
{
public:
    void Initialize() override;
    void Terminate() override;
    void Update(float deltaTime) override;
    void Render() override;
    void DebugUI() override;


    struct Object
    {
        Engine3D::Math::Matrix4 worldMat = Engine3D::Math::Matrix4::Identity;
        Engine3D::Graphics::MeshBuffer meshBuffer;
        Engine3D::Graphics::TextureId textureId;
        Engine3D::Graphics::Sampler sampler;
    };

    struct Object2
    {
        Engine3D::Math::Matrix4 worldMat = Engine3D::Math::Matrix4::Identity;
        Engine3D::Graphics::MeshBuffer meshBuffer;
        Engine3D::Graphics::TextureId textureId;
        Engine3D::Graphics::Sampler sampler;
    };

private:
    void UpdateCamera(float deltaTime);
    void RenderObject(const Object& object, const Engine3D::Graphics::Camera& camera);
    Engine3D::Graphics::Camera mCamera;
    Engine3D::Graphics::Camera mRenderTargetCamera;

    //GPU Communication
    Engine3D::Graphics::ConstantBuffer mTransformBuffer;
    Engine3D::Graphics::VertexShader mVertexShader;
    Engine3D::Graphics::PixelShader mPixelShader;
    Engine3D::Graphics::Sampler mSampler;

    //object
    Engine3D::Math::Matrix4 mWorldMat = Engine3D::Math::Matrix4::Identity;
    Engine3D::Graphics::MeshBuffer mObject;
    Engine3D::Graphics::TextureId mTextureId;

    //object 2
    Engine3D::Math::Matrix4 mWorldMat2 = Engine3D::Math::Matrix4::Identity;
    Engine3D::Graphics::MeshBuffer mObject2;
    Engine3D::Graphics::TextureId mTextureId2;

    //render target
    Engine3D::Graphics::RenderTarget mRenderTarget;
};