#pragma once

#include <3DEngine/Inc/3DEngine.h>

#include <atomic>
#include <cstddef>
#include <condition_variable>
#include <filesystem>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

class GridPlacementController;

class AsyncLoadController final
{
public:
    void Initialize(Engine3D::GameWorld& gameWorld, GridPlacementController& gridPlacementController);
    void Terminate();

    void Update(float deltaTime);
    void DebugUI();

private:
    enum class CellState
    {
        Unloaded,
        LoadingQueued,
        Loaded,
        UnloadingQueued
    };

    struct PersistedTransform
    {
        Engine3D::Math::Vector3 position = Engine3D::Math::Vector3::Zero;
        Engine3D::Math::Quaternion rotation = Engine3D::Math::Quaternion::Identity;
        Engine3D::Math::Vector3 scale = Engine3D::Math::Vector3::One;
    };

    struct Cell
    {
        std::string name;
        std::filesystem::path templatePath;
        PersistedTransform persistedTransform;

        CellState state = CellState::Unloaded;
        Engine3D::GameObject* gameObject = nullptr;
        Engine3D::GameObjectHandle handle;
    };

    struct CellSnapshot
    {
        Engine3D::Math::Vector3 position = Engine3D::Math::Vector3::Zero;
        CellState state = CellState::Unloaded;
    };

    struct WorkerRequest
    {
        Engine3D::Math::Vector3 cameraPosition = Engine3D::Math::Vector3::Zero;
        float loadRadius = 8.0f;
        std::vector<CellSnapshot> cellSnapshots;
    };

    struct WorkerResult
    {
        std::vector<size_t> loadIndices;
        std::vector<size_t> unloadIndices;
    };

    void BuildCells();
    void CapturePersistedTransforms();
    void SubmitWorkerRequest();
    void ApplyWorkerResult();
    void ProcessUnloadQueue();
    void ProcessLoadQueue();

    void StartWorkerThread();
    void StopWorkerThread();
    void WorkerMain();

    static float ComputeDistanceSqrXZ(const Engine3D::Math::Vector3& a, const Engine3D::Math::Vector3& b);

    Engine3D::GameWorld* mGameWorld = nullptr;
    GridPlacementController* mGridPlacementController = nullptr;

    std::vector<Cell> mCells;

    float mLoadRadius = 8.0f;
    int mMaxLoadsPerFrame = 8;
    int mMaxUnloadsPerFrame = 8;

    std::atomic<bool> mWorkerRunning = false;
    std::thread mWorkerThread;

    std::mutex mRequestMutex;
    std::condition_variable mRequestCv;
    WorkerRequest mPendingRequest;
    bool mHasPendingRequest = false;

    std::mutex mResultMutex;
    WorkerResult mPendingResult;
    bool mHasPendingResult = false;
};
