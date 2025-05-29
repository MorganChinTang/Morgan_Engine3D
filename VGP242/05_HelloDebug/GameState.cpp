#include "GameState.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;

void GameState::Initialize()
{
    mCamera.SetPosition({ 0.0f, 1.0f, -5.0f });
    mCamera.SetLookAt({ 0.0f, 0.0f, 0.0f });
}

void GameState::Terminate()
{

}

void GameState::Update(float deltaTime)
{
    UpdateCamera(deltaTime);
}

void GameState::Render()
{

}

bool gCheckValue = false;
float gFloatValue = 0.0f;
Math::Vector3 gVectorVal = Math::Vector3::Zero;
Math::Vector3 gV0 = Math::Vector3::Zero;
Math::Vector3 gV1 = Math::Vector3::One;
Math::Vector3 gV2 = Math::Vector3::XAxis;

Color gColor = Colors::LightPink;
Color gVectorV = Colors::White;
enum class Shape
{
    None,
    AABB,
    AABBFilled,
    Sphere,
    GroundPlane,
    GroundCircle,
    Transform
};

const char* gShapeNames[] =
{
    "None",
    "AABB",
    "AABBFilled",
    "Sphere",
    "GroundPlane",
    "GroundCircle",
    "Transform"
};

Shape gCurrentShape = Shape::None;

void GameState::DebugUI()
{
    //To Create A Window
    ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
    ImGui::Text("Hello World");
    ImGui::Checkbox("CheckCheck",&gCheckValue);
    ImGui::DragFloat("FloatVal", &gFloatValue);
    ImGui::DragFloat3("V0", &gV0.x, 0.1f);
    ImGui::DragFloat3("V1", &gV1.x, 0.1f);
    ImGui::DragFloat3("V2", &gV2.x, 0.1f);
    ImGui::ColorEdit4("Color", &gVectorV.r);

    int currentShape = (int)(gCurrentShape);
    if (ImGui::Combo("Shape", &currentShape, gShapeNames, std::size(gShapeNames)))
    {
        gCurrentShape = (Shape)currentShape;
    }

    ImGui::End();
    //
    switch (gCurrentShape)
    {
    case Shape::None: break;
    case Shape::AABB:
    {
        //ImGui::DragFloat("Min");
        //ImGui::DragFloat("Max");
        SimpleDraw::AddAABB(gV0, gV1, gColor);
        break;
    }
    case Shape::AABBFilled:
    {
        //ImGui::DragFloat("Min");
        //ImGui::DragFloat("Max");
        SimpleDraw::AddFilledAABB(gV0, gV1, gColor);
        break;
    }
    case Shape::Sphere:
    {
        //ImGui::DragFloat("Min");
        //ImGui::DragFloat("Max");
        SimpleDraw::AddSphere(60, 60, gFloatValue, gColor, gV0);
        break;
    }
    case Shape::GroundPlane:
    {
        //ImGui::DragFloat("Min");
        //ImGui::DragFloat("Max");
        SimpleDraw::AddGroundPlane(10.0f, gColor);
        break;
    }
    case Shape::GroundCircle:
    {
        //ImGui::DragFloat("Min");
        //ImGui::DragFloat("Max");
        SimpleDraw::AddGroundCircle(60, gFloatValue, gColor, gV0);
        break;
    }
    case Shape::Transform:
    {
        SimpleDraw::AddTransform(Math::Matrix4::Identity);
        break;
    }
    }
    SimpleDraw::AddLine(gV0,gV1, Colors::LightBlue);
    SimpleDraw::AddGroundPlane(10.0f, Colors::LightGoldenrodYellow);
    SimpleDraw::Render(mCamera);
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

    //if (input->IsKeyPressed(KeyCode::NUMPAD1))
    //{
    //    mMesh = MeshBuilder::CreateSpherePX(10, 10, 1.0f);
    //    mMeshBuffer.Initialize(mMesh);
    //}
}