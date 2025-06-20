#include "GameState.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;

struct Object
{
    MeshPX mesh;
    uint32_t textureId;
    Math::Matrix4 matWorld;
};

Object mSun;
SimpleTextureEffect mSimpleTextureEffect;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 10.0f, 30.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mRenderTargetCamera.SetPosition({ 0.0f, 10.0f, 30.0f });
    mRenderTargetCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
    mRenderTargetCamera.SetAspectRatio(1.0f);

    mRenderTargetCamera.SetNearPlane(1.0f);
    mRenderTargetCamera.SetFarPlane(1000.0f);
    
    // Initialize RenderTarget
    mRenderTarget.Initialize(128, 128, RenderTarget::Format::RGBA_U8);

    mSimpleTextureEffect.Initialize();
    mSimpleTextureEffect.SetCamera(mCamera);

    MeshPX sphere = MeshBuilder::CreateSpherePX(30, 30, 1.0f);
    // Sky Sphere Space
    MeshPX skySphere = MeshBuilder::CreateSkySpherePX(30, 30, 500.0f);
    mSkySphere.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/space.jpg");
    mSkySphere.renderData.mesh.Initialize(skySphere);

    // Sun
    mSun.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/sun.jpg");
    mSun.renderData.mesh.Initialize(sphere);
    mSun.renderData.matWorld = Math::Matrix4::Scaling(10.0f);

    // Mercury
    mMercury.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/mercury.jpg");
    mMercury.renderData.mesh.Initialize(sphere);
    mMercury.radius = 0.35f;
    mMercury.distanceFromSun = 12.0f;
    mMercury.orbitSpeed = 4.74f;
    mMercury.rotSpeed = 0.017f;

    // Venus
    mVenus.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/venus.jpg");
    mVenus.renderData.mesh.Initialize(sphere);
    mVenus.radius = 0.87f;
    mVenus.distanceFromSun = 16.0f;
    mVenus.orbitSpeed = 3.50f;
    mVenus.rotSpeed = 0.004f;

    // Earth
    mEarth.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/earth.jpg");
    mEarth.renderData.mesh.Initialize(sphere);
    mEarth.radius = 0.91f;
    mEarth.distanceFromSun = 20.0f;
    mEarth.orbitSpeed = 2.98f;
    mEarth.rotSpeed = 1.0f;

    // Moon (orbits Earth)
    mMoon.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/pluto.jpg");
    mMoon.renderData.mesh.Initialize(sphere);
    mMoon.radius = 0.25f;
    mMoon.distanceFromSun = 2.5f;  // Distance from Earth, not Sun directly
    mMoon.orbitSpeed = 5.0f;       // Fast orbit around Earth
    mMoon.rotSpeed = 0.2f;         // Slower rotation

    // Mars
    mMars.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/mars.jpg");
    mMars.renderData.mesh.Initialize(sphere);
    mMars.radius = 0.48f;
    mMars.distanceFromSun = 30.0f;
    mMars.orbitSpeed = 2.41f;
    mMars.rotSpeed = 0.97f;

    // Jupiter
    mJupiter.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/jupiter.jpg");
    mJupiter.renderData.mesh.Initialize(sphere);
    mJupiter.radius = 7.2f;
    mJupiter.distanceFromSun = 40.0f;
    mJupiter.orbitSpeed = 1.31f;
    mJupiter.rotSpeed = 2.42f;

    // Saturn
    mSaturn.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/saturn.jpg");
    mSaturn.renderData.mesh.Initialize(sphere);
    mSaturn.radius = 4.58f;
    mSaturn.distanceFromSun = 50.0f;
    mSaturn.orbitSpeed = 0.97f;
    mSaturn.rotSpeed = 2.25f;

    // Uranus
    mUranus.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/uranus.jpg");
    mUranus.renderData.mesh.Initialize(sphere);
    mUranus.radius = 2.66f;
    mUranus.distanceFromSun = 55.0f;
    mUranus.orbitSpeed = 0.68f;
    mUranus.rotSpeed = 1.41f;

    // Neptune
    mNeptune.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/neptune.jpg");
    mNeptune.renderData.mesh.Initialize(sphere);
    mNeptune.radius = 2.54f;
    mNeptune.distanceFromSun = 60.0f;
    mNeptune.orbitSpeed = 0.54f;
    mNeptune.rotSpeed = 1.50f;

    // Pluto
    mPluto.renderData.textureId = TextureManager::Get()->LoadTexture(L"../../Assets/Textures/planets/pluto.jpg");
    mPluto.renderData.mesh.Initialize(sphere);
    mPluto.radius = 0.17f;
    mPluto.distanceFromSun = 65.0f;
    mPluto.orbitSpeed = 0.47f;
    mPluto.rotSpeed = 0.16f;

}


void GameState::Terminate()
{
    mSimpleTextureEffect.Terminate();
    
    // Release textures
    TextureManager::Get()->ReleaseTexture(mSun.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mMercury.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mVenus.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mEarth.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mMars.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mJupiter.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mSaturn.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mUranus.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mNeptune.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mPluto.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mPluto.renderData.textureId);
    TextureManager::Get()->ReleaseTexture(mSkySphere.renderData.textureId);
    
    // Terminate meshes
    mSun.renderData.mesh.Terminate();
    mMercury.renderData.mesh.Terminate();
    mVenus.renderData.mesh.Terminate();
    mEarth.renderData.mesh.Terminate();
    mMars.renderData.mesh.Terminate();
    mJupiter.renderData.mesh.Terminate();
    mSaturn.renderData.mesh.Terminate();
    mUranus.renderData.mesh.Terminate();
    mNeptune.renderData.mesh.Terminate();
    mPluto.renderData.mesh.Terminate();
    mSkySphere.renderData.mesh.Terminate();
    mRenderTarget.Terminate();
}

bool gShowRings = false;
void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);

    
    // Sun stays at origin
    mSun.renderData.matWorld = Math::Matrix4::Scaling(10.0f);

    // Mercury
    mMercuryAngle += deltaTime * (mMercury.orbitSpeed + orbitSpeedChange);
    if (mMercuryAngle > Math::Constants::TwoPi)
        mMercuryAngle -= Math::Constants::TwoPi;
        
    float mercuryRotAngle = deltaTime * (mMercury.rotSpeed + rotSpeedChange);
    Math::Vector3 mercuryPos = {
        mMercury.distanceFromSun * std::cos(mMercuryAngle),
        0.0f,
        mMercury.distanceFromSun * std::sin(mMercuryAngle)
    };
    mMercury.renderData.matWorld = Math::Matrix4::RotationY(mercuryRotAngle) *
                                  Math::Matrix4::Scaling(mMercury.radius) *
                                  Math::Matrix4::Translation(mercuryPos);

    // Venus
    mVenusAngle += deltaTime * (mVenus.orbitSpeed + orbitSpeedChange);
    if (mVenusAngle > Math::Constants::TwoPi)
        mVenusAngle -= Math::Constants::TwoPi;
        
    float venusRotAngle = deltaTime * (mVenus.rotSpeed + rotSpeedChange);
    Math::Vector3 venusPos = {
        mVenus.distanceFromSun * std::cos(mVenusAngle),
        0.0f,
        mVenus.distanceFromSun * std::sin(mVenusAngle)
    };
    mVenus.renderData.matWorld = Math::Matrix4::RotationY(venusRotAngle) *
                               Math::Matrix4::Scaling(mVenus.radius) *
                               Math::Matrix4::Translation(venusPos);

    // Earth
    mEarthAngle += deltaTime * (mEarth.orbitSpeed + orbitSpeedChange);
    if (mEarthAngle > Math::Constants::TwoPi)
        mEarthAngle -= Math::Constants::TwoPi;
        
    float earthRotAngle = deltaTime * (mEarth.rotSpeed + rotSpeedChange);
    Math::Vector3 earthPos = {
        mEarth.distanceFromSun * std::cos(mEarthAngle),
        0.0f,
        mEarth.distanceFromSun * std::sin(mEarthAngle)
    };
    mEarth.renderData.matWorld = Math::Matrix4::RotationY(earthRotAngle) *
                               Math::Matrix4::Scaling(mEarth.radius) *
                               Math::Matrix4::Translation(earthPos);

    // Moon - orbits around Earth
    mMoonAngle += deltaTime * (mMoon.orbitSpeed + orbitSpeedChange);
    if (mMoonAngle > Math::Constants::TwoPi)
        mMoonAngle -= Math::Constants::TwoPi;

    // Calculate Moon's position relative to Earth's current position
    Math::Vector3 moonPos = {
        earthPos.x + mMoon.distanceFromSun * std::cos(mMoonAngle),
        0.0f,
        earthPos.z + mMoon.distanceFromSun * std::sin(mMoonAngle)
    };

    mMoon.renderData.matWorld = Math::Matrix4::RotationY(deltaTime * (mMoon.rotSpeed + rotSpeedChange)) *
                               Math::Matrix4::Scaling(mMoon.radius) *
                               Math::Matrix4::Translation(moonPos);

    // Mars
    mMarsAngle += deltaTime * (mMars.orbitSpeed + orbitSpeedChange);
    if (mMarsAngle > Math::Constants::TwoPi)
        mMarsAngle -= Math::Constants::TwoPi;
        
    float marsRotAngle = deltaTime * (mMars.rotSpeed + rotSpeedChange);
    Math::Vector3 marsPos = {
        mMars.distanceFromSun * std::cos(mMarsAngle),
        0.0f,
        mMars.distanceFromSun * std::sin(mMarsAngle)
    };
    mMars.renderData.matWorld = Math::Matrix4::RotationY(marsRotAngle) *
                              Math::Matrix4::Scaling(mMars.radius) *
                              Math::Matrix4::Translation(marsPos);

    // Jupiter
    mJupiterAngle += deltaTime * (mJupiter.orbitSpeed + orbitSpeedChange);
    if (mJupiterAngle > Math::Constants::TwoPi)
        mJupiterAngle -= Math::Constants::TwoPi;
        
    float jupiterRotAngle = deltaTime * (mJupiter.rotSpeed + rotSpeedChange);
    Math::Vector3 jupiterPos = {
        mJupiter.distanceFromSun * std::cos(mJupiterAngle),
        0.0f,
        mJupiter.distanceFromSun * std::sin(mJupiterAngle)
    };
    mJupiter.renderData.matWorld = Math::Matrix4::RotationY(jupiterRotAngle) *
                                 Math::Matrix4::Scaling(mJupiter.radius) *
                                 Math::Matrix4::Translation(jupiterPos);

    // Saturn
    mSaturnAngle += deltaTime * (mSaturn.orbitSpeed + orbitSpeedChange);
    if (mSaturnAngle > Math::Constants::TwoPi)
        mSaturnAngle -= Math::Constants::TwoPi;
        
    float saturnRotAngle = deltaTime * (mSaturn.rotSpeed + rotSpeedChange);
    Math::Vector3 saturnPos = {
        mSaturn.distanceFromSun * std::cos(mSaturnAngle),
        0.0f,
        mSaturn.distanceFromSun * std::sin(mSaturnAngle)
    };
    mSaturn.renderData.matWorld = Math::Matrix4::RotationY(saturnRotAngle) *
                                Math::Matrix4::Scaling(mSaturn.radius) *
                                Math::Matrix4::Translation(saturnPos);

    // Uranus
    mUranusAngle += deltaTime * (mUranus.orbitSpeed + orbitSpeedChange);
    if (mUranusAngle > Math::Constants::TwoPi)
        mUranusAngle -= Math::Constants::TwoPi;
        
    float uranusRotAngle = deltaTime * (mUranus.rotSpeed + rotSpeedChange);
    Math::Vector3 uranusPos = {
        mUranus.distanceFromSun * std::cos(mUranusAngle),
        0.0f,
        mUranus.distanceFromSun * std::sin(mUranusAngle)
    };
    mUranus.renderData.matWorld = Math::Matrix4::RotationY(uranusRotAngle) *
                                Math::Matrix4::Scaling(mUranus.radius) *
                                Math::Matrix4::Translation(uranusPos);

    // Neptune
    mNeptuneAngle += deltaTime * (mNeptune.orbitSpeed + orbitSpeedChange);
    if (mNeptuneAngle > Math::Constants::TwoPi)
        mNeptuneAngle -= Math::Constants::TwoPi;
        
    float neptuneRotAngle = deltaTime * (mNeptune.rotSpeed + rotSpeedChange);
    Math::Vector3 neptunePos = {
        mNeptune.distanceFromSun * std::cos(mNeptuneAngle),
        0.0f,
        mNeptune.distanceFromSun * std::sin(mNeptuneAngle)
    };
    mNeptune.renderData.matWorld = Math::Matrix4::RotationY(neptuneRotAngle) *
                                 Math::Matrix4::Scaling(mNeptune.radius) *
                                 Math::Matrix4::Translation(neptunePos);

    // Pluto
    mPlutoAngle += deltaTime * (mPluto.orbitSpeed + orbitSpeedChange);
    if (mPlutoAngle > Math::Constants::TwoPi)
        mPlutoAngle -= Math::Constants::TwoPi;
        
    float plutoRotAngle = deltaTime * (mPluto.rotSpeed + rotSpeedChange);
    Math::Vector3 plutoPos = {
        mPluto.distanceFromSun * std::cos(mPlutoAngle),
        0.0f,
        mPluto.distanceFromSun * std::sin(mPlutoAngle)
    };
    mPluto.renderData.matWorld = Math::Matrix4::RotationY(plutoRotAngle) *
                               Math::Matrix4::Scaling(mPluto.radius) *
                               Math::Matrix4::Translation(plutoPos);
}

void GameState::Render()
{

    mRenderTarget.BeginRender();
    
    mSimpleTextureEffect.SetCamera(mRenderTargetCamera);
    

    mSimpleTextureEffect.Begin();
    mSimpleTextureEffect.Render(mSun.renderData);
    mSimpleTextureEffect.Render(mMercury.renderData);
    mSimpleTextureEffect.Render(mVenus.renderData);
    mSimpleTextureEffect.Render(mEarth.renderData);
    mSimpleTextureEffect.Render(mMoon.renderData);
    mSimpleTextureEffect.Render(mMars.renderData);
    mSimpleTextureEffect.Render(mJupiter.renderData);
    mSimpleTextureEffect.Render(mSaturn.renderData);
    mSimpleTextureEffect.Render(mUranus.renderData);
    mSimpleTextureEffect.Render(mNeptune.renderData);
    mSimpleTextureEffect.Render(mPluto.renderData);
    mSimpleTextureEffect.Render(mSkySphere.renderData);
    mSimpleTextureEffect.End();
    

    mRenderTarget.EndRender();
    
    mSimpleTextureEffect.SetCamera(mCamera);
    
    mSimpleTextureEffect.Begin();
    mSimpleTextureEffect.Render(mSun.renderData);
    mSimpleTextureEffect.Render(mMercury.renderData);
    mSimpleTextureEffect.Render(mVenus.renderData);
    mSimpleTextureEffect.Render(mEarth.renderData);
    mSimpleTextureEffect.Render(mMoon.renderData);
    mSimpleTextureEffect.Render(mMars.renderData);
    mSimpleTextureEffect.Render(mJupiter.renderData);
    mSimpleTextureEffect.Render(mSaturn.renderData);
    mSimpleTextureEffect.Render(mUranus.renderData);
    mSimpleTextureEffect.Render(mNeptune.renderData);
    mSimpleTextureEffect.Render(mPluto.renderData);
    mSimpleTextureEffect.Render(mSkySphere.renderData);
    mSimpleTextureEffect.End();
    

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

bool gCheckValue = false;
float gFloatValue = 0.0f;
Math::Vector3 gVectorVal = Math::Vector3::Zero;
Math::Vector3 gV0 = Math::Vector3::Zero;
Math::Vector3 gV1 = Math::Vector3::One;
Math::Vector3 gV2 = Math::Vector3::XAxis;
Color gDisplayColor = Colors::White;
Color gColor = Colors::LightPink;
Color gVectorV = Colors::White;
enum class PlanetFocus
{
    None,
    Sun,
    Mercury,
    Venus,
    Earth,
    Moon,
    Mars,
    Jupiter,
    Saturn,
    Uranus,
    Neptune,
    Pluto
};

const char* gPlanetNames[] =
{
    "None",
    "Sun",
    "Mercury",
    "Venus",
    "Earth",
    "Moon",
    "Mars",
    "Jupiter",
    "Saturn",
    "Uranus",
    "Neptune",
    "Pluto"
};

PlanetFocus gCurrentFocus = PlanetFocus::None;

void GameState::DebugUI()
{
    ImGui::Begin("Solar System");
    
    // Orbit and rotation speed controls (global)
    ImGui::SliderFloat("Orbit Speed Change", &orbitSpeedChange, -5.0f, 5.0f);
    ImGui::SliderFloat("Rotation Speed Change", &rotSpeedChange, -5.0f, 5.0f);
    ImGui::Checkbox("Show Rings", &gShowRings);

    if (gShowRings)
    {
        // Draw orbit rings for main view
        SimpleDraw::AddGroundCircle(60, mMercury.distanceFromSun, Colors::Gray, Math::Vector3::Zero);
        SimpleDraw::AddGroundCircle(60, mVenus.distanceFromSun, Colors::Gray, Math::Vector3::Zero);
        SimpleDraw::AddGroundCircle(60, mEarth.distanceFromSun, Colors::Gray, Math::Vector3::Zero);
        SimpleDraw::AddGroundCircle(60, mMars.distanceFromSun, Colors::Gray, Math::Vector3::Zero);
        SimpleDraw::AddGroundCircle(60, mJupiter.distanceFromSun, Colors::Gray, Math::Vector3::Zero);
        SimpleDraw::AddGroundCircle(60, mSaturn.distanceFromSun, Colors::Gray, Math::Vector3::Zero);
        SimpleDraw::AddGroundCircle(60, mUranus.distanceFromSun, Colors::Gray, Math::Vector3::Zero);
        SimpleDraw::AddGroundCircle(60, mNeptune.distanceFromSun, Colors::Gray, Math::Vector3::Zero);
        SimpleDraw::AddGroundCircle(60, mPluto.distanceFromSun, Colors::Gray, Math::Vector3::Zero);
        SimpleDraw::Render(mCamera);
    }
    
    // Planet focus selection
    int currentPlanet = static_cast<int>(gCurrentFocus);
    if (ImGui::Combo("Planet", &currentPlanet, gPlanetNames, std::size(gPlanetNames)))
    {
        gCurrentFocus = static_cast<PlanetFocus>(currentPlanet);
    }

    // Planet-specific controls based on selection
    switch (gCurrentFocus)
    {
    case PlanetFocus::Sun:
        ImGui::Text("Sun");
        mRenderTargetCamera.SetLookAt({0.0f, 0.0f, 0.0f});
        break;
    case PlanetFocus::Mercury:
        {
            ImGui::Text("Mercury");
            ImGui::DragFloat("Orbit Translation Speed", &mMercury.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            Math::Vector3 pos = {
                mMercury.distanceFromSun * std::cos(mMercuryAngle),
                0.0f,
                mMercury.distanceFromSun * std::sin(mMercuryAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    case PlanetFocus::Venus:
        {
            ImGui::Text("Venus");
            ImGui::DragFloat("Orbit Translation Speed", &mVenus.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            Math::Vector3 pos = {
                mVenus.distanceFromSun * std::cos(mVenusAngle),
                0.0f,
                mVenus.distanceFromSun * std::sin(mVenusAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    case PlanetFocus::Earth:
        {
            ImGui::Text("Earth");
            ImGui::DragFloat("Orbit Translation Speed", &mEarth.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            Math::Vector3 pos = {
                mEarth.distanceFromSun * std::cos(mEarthAngle),
                0.0f,
                mEarth.distanceFromSun * std::sin(mEarthAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    case PlanetFocus::Moon:
        {
            ImGui::Text("Moon");
            ImGui::DragFloat("Orbit Translation Speed", &mMoon.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            // Calculate Earth's position first
            Math::Vector3 earthPos = {
                mEarth.distanceFromSun * std::cos(mEarthAngle),
                0.0f,
                mEarth.distanceFromSun * std::sin(mEarthAngle)
            };
            
            // Then calculate Moon's position relative to Earth
            Math::Vector3 pos = {
                earthPos.x + mMoon.distanceFromSun * std::cos(mMoonAngle),
                0.0f,
                earthPos.z + mMoon.distanceFromSun * std::sin(mMoonAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    case PlanetFocus::Mars:
        {
            ImGui::Text("Mars");
            ImGui::DragFloat("Orbit Translation Speed", &mMars.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            Math::Vector3 pos = {
                mMars.distanceFromSun * std::cos(mMarsAngle),
                0.0f,
                mMars.distanceFromSun * std::sin(mMarsAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    case PlanetFocus::Jupiter:
        {
            ImGui::Text("Jupiter");
            ImGui::DragFloat("Orbit Translation Speed", &mJupiter.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            Math::Vector3 pos = {
                mJupiter.distanceFromSun * std::cos(mJupiterAngle),
                0.0f,
                mJupiter.distanceFromSun * std::sin(mJupiterAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    case PlanetFocus::Saturn:
        {
            ImGui::Text("Saturn");
            ImGui::DragFloat("Orbit Translation Speed", &mSaturn.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            Math::Vector3 pos = {
                mSaturn.distanceFromSun * std::cos(mSaturnAngle),
                0.0f,
                mSaturn.distanceFromSun * std::sin(mSaturnAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    case PlanetFocus::Uranus:
        {
            ImGui::Text("Uranus");
            ImGui::DragFloat("Orbit Translation Speed", &mUranus.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            Math::Vector3 pos = {
                mUranus.distanceFromSun * std::cos(mUranusAngle),
                0.0f,
                mUranus.distanceFromSun * std::sin(mUranusAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    case PlanetFocus::Neptune:
        {
            ImGui::Text("Neptune");
            ImGui::DragFloat("Orbit Translation Speed", &mNeptune.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            Math::Vector3 pos = {
                mNeptune.distanceFromSun * std::cos(mNeptuneAngle),
                0.0f,
                mNeptune.distanceFromSun * std::sin(mNeptuneAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    case PlanetFocus::Pluto:
        {
            ImGui::Text("Pluto");
            ImGui::DragFloat("Orbit Translation Speed", &mPluto.orbitSpeed, 0.01f, 0.0f, 10.0f);
            
            Math::Vector3 pos = {
                mPluto.distanceFromSun * std::cos(mPlutoAngle),
                0.0f,
                mPluto.distanceFromSun * std::sin(mPlutoAngle)
            };
            mRenderTargetCamera.SetLookAt(pos);
        }
        break;
    default:
        // None selected
        mRenderTargetCamera.SetLookAt({0.0f, 0.0f, 0.0f});
        break;
    }
    
    // Add separator and render target display in the same window
    ImGui::Separator();
    ImGui::Text("Render Target");
    ImGui::Image(
        mRenderTarget.GetRawData(),
        { 128, 128 },
        { 0, 0 },
        { 1, 1 },
        { 1, 1, 1, 1 },
        { 1, 1, 1, 1 }
    );
    
    ImGui::End();
}
