#include "Precompiled.h"
#include "SimpleTextureEffect.h"
#include "TextureManager.h"
#include "VertexTypes.h"
#include "Camera.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;

void SimpleTextureEffect::Initialize()
{
    std::filesystem::path shaderPath = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderPath);
    mPixelShader.Initialize(shaderPath);
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
    mTransformBuffer.Initialize(sizeof(Math::Matrix4));
}

void SimpleTextureEffect::Terminate()
{
    mTransformBuffer.Terminate();
    mSampler.Terminate();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
}
 
void SimpleTextureEffect::Begin()
{
    mVertexShader.Bind();
    mPixelShader.Bind();
    mSampler.BindPS(0);
    mTransformBuffer.BindVS(0);
}

void SimpleTextureEffect::End()
{
    Texture::UnbindPS(0);
}

void SimpleTextureEffect::Render(const SimpleTextureEffect::RenderData& renderData)
{
    ASSERT(mCamera != nullptr, "SimpleTextureEffect: Must have a Camera");
    const Math::Matrix4 matView = mCamera->GetViewMatrix();
    const Math::Matrix4 matProj = mCamera->GetProjectionMatrix();
    const Math::Matrix4 matFinal = renderData.matWorld * matView * matProj;
    const Math::Matrix4 wvp = Math::Transpose(matFinal);
    mTransformBuffer.Update(&wvp);

    TextureManager::Get()->BindPS(renderData.textureId, 0);
    renderData.mesh.Render();
}

void SimpleTextureEffect::SetCamera(const Camera& camera)
{
    mCamera = &camera;
}
