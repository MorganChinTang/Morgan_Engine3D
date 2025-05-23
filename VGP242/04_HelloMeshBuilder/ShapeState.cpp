#include "ShapeState.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Math;

void ShapeState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -3.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });

    mTransformBuffer.Initialize(sizeof(Math::Matrix4));
    // creates a shape out of the vertices
    CreateShape();
    mMeshBuffer.Initialize(mMesh);

    std::filesystem::path shaderFilePath = L"../../Assets/Shaders/DoTexture.fx";
    mVertexShader.Initialize<VertexPX>(shaderFilePath);
    mPixelShader.Initialize(shaderFilePath);

    mTexture.Initialize(L"../../Assets/Textures/skysphere/sky.jpg");
    mSampler.Initialize(Sampler::Filter::Linear, Sampler::AddressMode::Wrap);
}

void ShapeState::Terminate()
{
    mMesh.vertices.clear();
    mPixelShader.Terminate();
    mVertexShader.Terminate();
    mMeshBuffer.Terminate();
    mTransformBuffer.Terminate();
    
    mTexture.Terminate();
    mSampler.Terminate();
}

void ShapeState::Update(float deltaTime)
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

    if (input->IsKeyPressed(KeyCode::NUMPAD1))
    {
        mMesh = MeshBuilder::CreateSpherePX(10, 10, 1.0f);
        mMeshBuffer.Initialize(mMesh);
    }
    if (input->IsKeyPressed(KeyCode::NUMPAD2))
    {
        mMesh = MeshBuilder::CreatePlanePX(3, 3, 1.0f, true);
        mMeshBuffer.Initialize(mMesh);
    }
    if (input->IsKeyPressed(KeyCode::NUMPAD3))
    {
        mMesh = MeshBuilder::CreatePlanePX(3, 3, 1.0f, false);
        mMeshBuffer.Initialize(mMesh);
    }
    if (input->IsKeyPressed(KeyCode::NUMPAD4))
    {
        mMesh = MeshBuilder::CreateSkySpherePX(10, 10, 200.0f);
        mMeshBuffer.Initialize(mMesh);
    }
}

void ShapeState::Render()
{
    //prepare GPU
    mVertexShader.Bind();
    mPixelShader.Bind();

    //bind texture
    mTexture.BindPS(0);
    mSampler.BindPS(0);

    //sync Transform buffer
    mTransformBuffer.BindVS(0);

    //update the buffer data
    Math::Matrix4 matWorld = Math::Matrix4::Identity;
    Math::Matrix4 matView = mCamera.GetViewMatrix();
    Math::Matrix4 matProj = mCamera.GetProjectionMatrix();
    Math::Matrix4 matFinal = matWorld * matView * matProj;
    Math::Matrix4 wvp = Math::Transpose(matFinal);
    mTransformBuffer.Update(&wvp);
    //draw
    mMeshBuffer.Render();
}

void ShapeState::CreateShape()
{/*
    mMesh = MeshBuilder::CreateCubePC(1.0f);
    mMesh = MeshBuilder::CreatePyramidPC(1.0f);
    mMesh = MeshBuilder::CreateRectanglePC(0.2f, 0.5f, 1.0f);
    mMesh = MeshBuilder::CreatePlanePC(10, 10, 1.0f, true);
    mMesh = MeshBuilder::CreateCylinderPC(10, 3);
    mMesh = MeshBuilder::CreateSpherePC(10, 10, 1.0f);*/

    //Hello MeshBuilder
    //mMesh = MeshBuilder::CreateSpherePX(10, 10, 1.0f);
    //mMesh = MeshBuilder::CreatePlanePX(10, 10, 1.0f, true);
    //mMesh = MeshBuilder::CreatePlanePX(10, 10, 1.0f, false);
    mMesh = MeshBuilder::CreateSkySpherePX(10, 10, 200.0f);
}