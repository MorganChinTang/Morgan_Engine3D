#include "Precompiled.h"
#include "Dissolve.h"

#include "RenderObject.h"
#include "Camera.h"
#include "GraphicsSystem.h"
#include "VertexTypes.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;

void DissolveEffect::Initialize()
{
	std::filesystem::path shaderFile = L"../../Assets/Shaders/Dissolve.fx";
	mVertexShader.Initialize<Vertex>(shaderFile);
	mPixelShader.Initialize(shaderFile);

	mTransformBuffer.Initialize();
	mLightBuffer.Initialize();
	mMaterialBuffer.Initialize();
	mSettingsBuffer.Initialize();
	mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}
void DissolveEffect::Terminate()
{
	mSampler.Terminate();
	mSettingsBuffer.Terminate();
	mMaterialBuffer.Terminate();
	mLightBuffer.Terminate();
	mTransformBuffer.Terminate();
	mPixelShader.Terminate();
	mVertexShader.Terminate();
}

void DissolveEffect::Begin()
{
	mVertexShader.Bind();
	mPixelShader.Bind();

	mTransformBuffer.BindVS(0);

	mLightBuffer.BindVS(1);
	mLightBuffer.BindPS(1);

	mMaterialBuffer.BindPS(2);

	mSettingsBuffer.BindVS(3);
	mSettingsBuffer.BindPS(3);

	mSampler.BindPS(0);
	mSampler.BindVS(0);
}
void DissolveEffect::End()
{
	if (mShadowMap != nullptr)
	{
		Texture::UnbindPS(2);
	}
}

void DissolveEffect::Render(const RenderObject& renderObject)
{
	ASSERT(mCamera != nullptr, "TerrainEffect: No camera specified.");
	ASSERT(mDirectionalLight != nullptr, "TerrainEffect: No light specified.");

	Math::Matrix4 matWorld = renderObject.transform.GetMatrix4();
	Math::Matrix4 matView = mCamera->GetViewMatrix();
	Math::Matrix4 matProj = mCamera->GetProjectionMatrix();

	TransformData data;
	data.world = Math::Transpose(matWorld);
	data.wvp = Math::Transpose(matWorld * matView * matProj);
	if (mShadowMap != nullptr && mLightCamera != nullptr)
	{
		matView = mLightCamera->GetViewMatrix();
		matProj = mLightCamera->GetProjectionMatrix();
		data.lwvp = Math::Transpose(matWorld * matView * matProj);
	}

	SettingsData settings;
	settings.useShadowMap = (mShadowMap != nullptr && mSettingsData.useShadowMap > 0) ? 1 : 0;
	settings.depthBias = mSettingsData.depthBias;
	settings.lowHeight = mSettingsData.lowHeight;
	settings.blendHeight = mSettingsData.blendHeight;

	mTransformBuffer.Update(data);
	mLightBuffer.Update(*mDirectionalLight);
	mMaterialBuffer.Update(renderObject.material);
	mSettingsBuffer.Update(settings);

	TextureManager* tm = TextureManager::Get();
	tm->BindPS(renderObject.diffuseMapId, 0);
	tm->BindPS(renderObject.specMapId, 1);
	if (mShadowMap != nullptr && settings.useShadowMap > 0)
	{
		mShadowMap->BindPS(2);
	}
	renderObject.meshBuffer.Render();
}
void DissolveEffect::DebugUI()
{
	if (ImGui::CollapsingHeader("TerrainEffect", ImGuiTreeNodeFlags_DefaultOpen))
	{
		bool useShadowMap = mSettingsData.useShadowMap > 0;
		if (ImGui::Checkbox("UseShadowMap##TerrainEffect", &useShadowMap))
		{
			mSettingsData.useShadowMap = useShadowMap ? 1 : 0;
		}
		ImGui::DragFloat("DepthBias##TerrainEffect", &mSettingsData.depthBias, 0.000001f, 0.0f, 1.0f, "%.6f");
		ImGui::DragFloat("LowHeight##TerrainEffect", &mSettingsData.lowHeight, 0.1f, 0.0f, 20.0f);
		ImGui::DragFloat("BlendHeight##TerrainEffect", &mSettingsData.blendHeight, 0.1f, 0.0f, 20.0f);
	}
}

void DissolveEffect::SetCamera(const Camera& camera)
{
	mCamera = &camera;
}
void DissolveEffect::SetLightCamera(const Camera& lightCamera)
{
	mLightCamera = &lightCamera;
}
void DissolveEffect::SetDirectionalLight(const DirectionalLight& directionalLight)
{
	mDirectionalLight = &directionalLight;
}
void DissolveEffect::SetShadowMap(const Texture& shadowMap)
{
	mShadowMap = &shadowMap;
}