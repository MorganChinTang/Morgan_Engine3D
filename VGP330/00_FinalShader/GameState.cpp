#include "GameState.h"
using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 5.0f, 9.5f, 0.0f });
    mCamera.SetLookAt({ 5.0f, 9.5f, 5.0f });

    mDirectionalLight.direction = Math::Normalize({ 1.0f,-1.0f, 1.0 });
    mDirectionalLight.ambient = { 1.5f,1.5f,1.5f,1.0f };
    mDirectionalLight.diffuse = { 2.2f,2.2f,2.2f,1.0f };
    mDirectionalLight.specular = { 2.2f,2.2f,2.2f,1.0f };

    mRenderTargetCamera.SetPosition({ 0.0f, 10.0f, 30.0f });
    mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetAspectRatio(1.0f);

    mRenderTargetCamera.SetNearPlane(1.0f);
    mRenderTargetCamera.SetFarPlane(1000.0f);

    // Initialize RenderTarget
    mRenderTarget.Initialize(128, 128, RenderTarget::Format::RGBA_U8);
    //Characters
    mCharacter1.Initialize(L"../../Assets/Models/Character01/Character01.model");
    mCharacter2.Initialize(L"../../Assets/Models/Character02/Character02.model");
    mCharacter3.Initialize(L"../../Assets/Models/Character03/Character03.model");

    // Sky Sphere Space
    MeshPX skySphere = MeshBuilder::CreateSkySpherePX(30, 30, 500.0f);
    mSkySphere.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/skysphere/sky_27_2k.png");
    mSkySphere.renderData.mesh.Initialize(skySphere);

    // Load two textures for Character1 blending
    //mCharacter1DiffuseMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/terrain/dirt_seamless.jpg");
    for (auto& renderObject : mCharacter1.renderObjects)
    {
        TextureManager::Get()->ReleaseTexture(renderObject.specMapId);
        renderObject.specMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/terrain/grass_2048.jpg");
    }

    for (auto& renderObject : mCharacter2.renderObjects)
    {
        TextureManager::Get()->ReleaseTexture(renderObject.specMapId);
        renderObject.specMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/terrain/grass_2048.jpg");
    }

    for (auto& renderObject : mCharacter3.renderObjects)
    {
        TextureManager::Get()->ReleaseTexture(renderObject.specMapId);
        renderObject.specMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/terrain/grass_2048.jpg");
    }

    mCharacter1.transform.position = { 4.0f, 8.45f, 5.0f };
    mCharacter2.transform.position = { 5.0f, 8.7f, 5.0f };
    mCharacter3.transform.position = { 6.5f, 8.0f, 5.0f };

    //Terrain
    mTerrain.Initialize(L"../../Assets/Textures/terrain/heightmap_512x512.raw", 20.0f);
    mGround.meshBuffer.Initialize(mTerrain.mesh);
    mGround.diffuseMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/terrain/dirt_seamless.jpg");
    mGround.specMapId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/terrain/grass_2048.jpg");

    std::filesystem::path shaderFile = L"../../Assets/Shaders/Standard.fx";
    mStandardEffect.Initialize(shaderFile);
    mStandardEffect.SetCamera(mCamera);
    mStandardEffect.SetDirectionalLight(mDirectionalLight);
    mStandardEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mStandardEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    mShadowEffect.Initialize();
    mShadowEffect.SetDirectionalLight(mDirectionalLight);

    mTerrainEffect.Initialize();
    mTerrainEffect.SetCamera(mCamera);
    mTerrainEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mTerrainEffect.SetDirectionalLight(mDirectionalLight);
    mTerrainEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    // Initialize DissolveEffect for Character1 with two textures
    mDissolveEffect.Initialize();
    mDissolveEffect.SetCamera(mCamera);
    mDissolveEffect.SetLightCamera(mShadowEffect.GetLightCamera());
    mDissolveEffect.SetDirectionalLight(mDirectionalLight);
    mDissolveEffect.SetShadowMap(mShadowEffect.GetDepthMap());

    // Initialize SimpleTextureEffect for sky sphere rendering
    mSimpleTextureEffect.Initialize();
    mSimpleTextureEffect.SetCamera(mCamera);
}
void GameState::Terminate()
{
    TextureManager::Get()->ReleaseTexture(mSkySphere.renderData.textureId);
    mSimpleTextureEffect.Terminate();
    mDissolveEffect.Terminate();
    mTerrainEffect.Terminate();
    mShadowEffect.Terminate();
    mStandardEffect.Terminate();
    mCharacter1.Terminate();
    mCharacter2.Terminate();
    mCharacter3.Terminate();
    mGround.Terminate();
    mSkySphere.renderData.mesh.Terminate();
    mRenderTarget.Terminate();
}
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

void GameState::Render()
{
    mRenderTarget.BeginRender();
    mShadowEffect.Begin();
    mShadowEffect.Render(mCharacter1);
    mShadowEffect.Render(mCharacter2);
    mShadowEffect.Render(mCharacter3);
    mShadowEffect.Render(mSphere1);
    mShadowEffect.Render(mSphere2);
    mShadowEffect.End();

    mTerrainEffect.Begin();
    mTerrainEffect.Render(mGround);
    mTerrainEffect.End();

    // Render Character1 with DissolveEffect (two texture blending)
    mDissolveEffect.Begin();
        mDissolveEffect.Render(mCharacter1);
        mDissolveEffect.Render(mCharacter2);
        mDissolveEffect.Render(mCharacter3);
    mDissolveEffect.End();

    mRenderTarget.EndRender();

    // Render sky sphere first (background)
    mSimpleTextureEffect.Begin();
    mSimpleTextureEffect.Render(mSkySphere.renderData);
    mSimpleTextureEffect.End();

    // Render terrain
    //mTerrainEffect.Begin();
    //mTerrainEffect.Render(mGround);
    //mTerrainEffect.End();

    // Render characters with DissolveEffect
    mDissolveEffect.Begin();
    mDissolveEffect.Render(mCharacter1);
    mDissolveEffect.Render(mCharacter2);
    mDissolveEffect.Render(mCharacter3);
    mDissolveEffect.End();

    mStandardEffect.Begin();
    mStandardEffect.End();
}

void GameState::DebugUI()
{
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_None))
    {
        if (ImGui::DragFloat3("DirectionalLight", &mDirectionalLight.direction.x, 0.01f))
        {
            mDirectionalLight.direction = Math::Normalize(mDirectionalLight.direction);
        }

        ImGui::ColorEdit4("Ambient#Light", &mDirectionalLight.ambient.r);
        ImGui::ColorEdit4("Diffuse#Light", &mDirectionalLight.diffuse.r);
        ImGui::ColorEdit4("Specular#Light", &mDirectionalLight.specular.r);
    }

    if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_None))
    {
        for (uint32_t i = 0; i < mCharacter1.renderObjects.size(); i++)
        {
            Material& material = mCharacter1.renderObjects[i].material;
            std::string renderObjectId = "RenderObject" + std::to_string(i);
            ImGui::PushID(renderObjectId.c_str());
            if (ImGui::CollapsingHeader("RenderObject"))
            {
                ImGui::LabelText("Label", "Material: ");
                ImGui::ColorEdit4("Emissive#Material", &material.emissive.r);
                ImGui::ColorEdit4("Ambient#Material", &material.ambient.r);
                ImGui::ColorEdit4("Diffuse#Material", &material.diffuse.r);
                ImGui::ColorEdit4("Specular#Material", &material.specular.r);
                ImGui::DragFloat("Shininess#Material", &material.shininess, 0.01f, 0.01f, 1000.0f);
            }
            ImGui::PopID();
        }
    }
    ImGui::DragFloat3("CharacterPosition1", &mCharacter1.transform.position.x);
    ImGui::DragFloat3("CharacterPosition2", &mCharacter2.transform.position.x);
    ImGui::DragFloat3("CharacterPosition3", &mCharacter3.transform.position.x);

    mStandardEffect.DebugUI();
    ImGui::Separator();
    mShadowEffect.DebugUI();
    mTerrainEffect.DebugUI();
    mDissolveEffect.DebugUI();
    ImGui::End();
}

void GameState::UpdateCamera(float deltaTime)
{
    InputSystem* input = InputSystem::Get();
    const float moveSpeed = input->IsKeyDown(KeyCode::LSHIFT) ? 10.0f : 1.0f;
    const float turnSpeed = 0.1f;
    if (input->IsKeyDown(KeyCode::W))
    {
        mCamera.Walk(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::S))
    {
        mCamera.Walk(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::D))
    {
        mCamera.Strafe(moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::A))
    {
        mCamera.Strafe(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::Q))
    {
        mCamera.Rise(-moveSpeed * deltaTime);
    }
    if (input->IsKeyDown(KeyCode::E))
    {
        mCamera.Rise(moveSpeed * deltaTime);
    }

    if (input->IsMouseDown(MouseButton::RBUTTON))
    {
        mCamera.Yaw(input->GetMouseMoveX() * turnSpeed * deltaTime);
        mCamera.Pitch(input->GetMouseMoveY() * turnSpeed * deltaTime);
    }

}

