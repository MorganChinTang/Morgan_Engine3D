#include "Precompiled.h"
#include "ModelManager.h"
#include "ModelIO.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;

namespace
{
	std::unique_ptr<ModelManager> sModelManager;
}
void ModelManager::StaticInitialize(const std::filesystem::path& rootPath)
{
	ASSERT(sModelManager == nullptr, "ModelManager: Is already initialized");
	sModelManager = std::make_unique<ModelManager>();
	sModelManager->SetRootDirectory(rootPath);
}
void ModelManager::StaticTerminate()
{
	sModelManager.reset();
}
ModelManager* ModelManager:: Get()
{
	ASSERT(sModelManager != nullptr, "ModelManager: Is not initialized");
	return sModelManager.get();
}

void ModelManager::SetRootDirectory(const std::filesystem::path& rootPath)
{
	mRootDirectory = rootPath;
}
ModelId ModelManager::GetModelId(const std::filesystem::path& filePath)
{
	return std::filesystem::hash_value(mRootDirectory / filePath);
}
ModelId ModelManager::LoadModel(const std::filesystem::path& filePath)
{
	const ModelId modelId = GetModelId(filePath);
	auto [iter, success] = mInventory.insert({ modelId, nullptr });
	if (success)
	{
		std::filesystem::path fullPath = mRootDirectory / filePath;
		auto& modelPtr = iter->second;
		modelPtr = std::make_unique<Model>();
		ModelIO::LoadModel(fullPath, *modelPtr);
		ModelIO::LoadMaterial(fullPath, *modelPtr);
		ModelIO::LoadSkeleton(fullPath, *modelPtr);
	}
	return modelId;
}
void ModelManager::AddAnimation(ModelId id, const std::filesystem::path& filePath)
{
	auto model = mInventory.find(id);
	ASSERT(model != mInventory.end(), "ModelManager: model not found for animation");
	ModelIO::LoadAnimation(filePath, *model->second);
}
const Model* ModelManager::GetModel(ModelId id)
{
	auto model = mInventory.find(id);
	if (model != mInventory.end())
	{
		return model->second.get();
	}
	return nullptr;
}