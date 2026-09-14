#include "GameState.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Input;
using namespace Engine3D::Physics;

namespace
{
	constexpr const char* MazeWallTemplate = "../../Assets/Templates/Objects/maze_wall.json";
	constexpr float CellSize = 4.0f;
	constexpr float WallHeight = 1.5f;
	constexpr float WallThickness = 0.4f;

	struct WallSegment
	{
		int x1, row1, x2, row2;
	};

	// Image grid: (0,0) is the top-left corner; x increases right, row increases down.
	// Top entrance is between x=2 and x=3; bottom exit is between x=3 and x=4.
	constexpr WallSegment MazeWalls[] =
	{
		{ 0, 0, 2, 0 }, { 3, 0, 6, 0 },
		{ 4, 1, 5, 1 },
		{ 1, 2, 3, 2 }, { 4, 2, 6, 2 },
		{ 1, 3, 4, 3 },
		{ 1, 4, 2, 4 }, { 4, 4, 5, 4 },
		{ 0, 5, 1, 5 }, { 3, 5, 4, 5 }, { 5, 5, 6, 5 },
		{ 0, 6, 3, 6 }, { 4, 6, 6, 6 },
		{ 0, 0, 0, 6 }, { 6, 0, 6, 6 },
		{ 2, 0, 2, 1 }, { 4, 0, 4, 1 },
		{ 1, 1, 1, 3 }, { 3, 1, 3, 2 },
		{ 5, 2, 5, 4 }, { 3, 3, 3, 5 },
		{ 2, 4, 2, 6 }, { 4, 5, 4, 6 }
	};

	void CreateMaze(GameWorld& world)
	{
		int index = 0;
		for (const WallSegment& wall : MazeWalls)
		{
			const Math::Vector3 position =
			{
				(wall.x1 + wall.x2) * CellSize * 0.5f - 3.0f * CellSize,
				WallHeight * 0.5f,
				3.0f * CellSize - (wall.row1 + wall.row2) * CellSize * 0.5f
			};
			const Math::Vector3 size =
			{
				(wall.x2 - wall.x1) * CellSize + WallThickness,
				WallHeight,
				(wall.row2 - wall.row1) * CellSize + WallThickness
			};
			GameObject* object = world.CreateGameObject("MazeWall_" + std::to_string(index++), MazeWallTemplate);
			if (object == nullptr)
			{
				return;
			}
			auto* transform = object->GetComponent<TransformComponent>();
			transform->position = position;
			transform->scale = size;

			// Match the static collider to the visible cube; visual scale does not resize Bullet shapes.
			rapidjson::Document body(rapidjson::kObjectType);
			rapidjson::Value collider(rapidjson::kObjectType);
			SaveUtil::WriteString("Shape", "Box", body, collider);
			SaveUtil::WriteVector3("HalfExtents", size * 0.5f, body, collider);
			body.AddMember("ColliderData", collider, body.GetAllocator());
			object->GetComponent<RigidBodyComponent>()->Deserialize(body);
			object->Initialize();
		}
	}
}

Service* MakeCustomService(const std::string& serviceName, GameWorld& gameWorld)
{
	
	return nullptr;
		
}

Component* MakeCustomComponent(const std::string& componentName, GameObject& gameObject)
{

	return nullptr;
}
Component* GetCustomComponent(const std::string& componentName, GameObject& gameObject)
{
	return nullptr;
}

void GameState::Initialize() 
{
	mLevelFile = L"../../Assets/Templates/Levels/level_mp.json";
	
	// set a callback to try make a custom service (any serice that is NOT part of the engine and unique to the project)
	GameWorld::SetCustomService(MakeCustomService);
	//sets callbacks to try make/get a custom component (any component that is NOT part of the engine)
	GameObjectFactory::SetCustomMake(MakeCustomComponent);
	GameObjectFactory::SetCustomGet(GetCustomComponent);

	mGameWorld.LoadLevel(mLevelFile);
	CreateMaze(mGameWorld);
}
void GameState::Terminate() 
{
	mGameWorld.Terminate();
}
void GameState::Update(float deltaTime) 
{
	mGameWorld.Update(deltaTime);
}
void GameState::Render() 
{
	mGameWorld.Render();

}

void GameState::DebugUI()
{
	ImGui::Begin("Debug", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	mGameWorld.DebugUI(false);
	ImGui::End();
}



