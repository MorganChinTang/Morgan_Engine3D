#include "AsyncLoadController.h"

#include "GridPlacementController.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;
using namespace Engine3D::Math;

namespace
{
    constexpr int kGridCountX = 5;
    constexpr int kGridCountZ = 5;
    constexpr float kGridSpacing = 2.0f;
    constexpr float kGridStartZ = -4.0f;
    constexpr float kCactusStartX = -10.0f;
    constexpr float kSaladStartX = 2.0f;

    constexpr const wchar_t* kCactusTemplate = L"../../Assets/Templates/Objects/furniture_cactus.json";
    constexpr const wchar_t* kSaladTemplate = L"../../Assets/Templates/Objects/furniture_salad.json";
}

void AsyncLoadController::Initialize(GameWorld& gameWorld, GridPlacementController& gridPlacementController)
{
    mGameWorld = &gameWorld;
    mGridPlacementController = &gridPlacementController;

    BuildCells();
    StartWorkerThread();
}

void AsyncLoadController::Terminate()
{
    StopWorkerThread();

    if (mGameWorld != nullptr && mGridPlacementController != nullptr)
    {
        for (Cell& cell : mCells)
        {
            if (cell.gameObject == nullptr)
            {
                continue;
            }

            if (TransformComponent* transformComponent = cell.gameObject->GetComponent<TransformComponent>())
            {
                cell.persistedTransform.position = transformComponent->position;
                cell.persistedTransform.rotation = transformComponent->rotation;
                cell.persistedTransform.scale = transformComponent->scale;
            }

            mGridPlacementController->UnregisterPickableFurniture(*cell.gameObject);
            mGameWorld->DestroyGameObject(cell.handle);
            cell.gameObject = nullptr;
            cell.state = CellState::Unloaded;
        }
    }

    mCells.clear();
    mGameWorld = nullptr;
    mGridPlacementController = nullptr;
}

void AsyncLoadController::Update(float)
{
    if (mGameWorld == nullptr || mGridPlacementController == nullptr)
    {
        return;
    }

    CapturePersistedTransforms();
    SubmitWorkerRequest();
    ApplyWorkerResult();

    // Engine game-object mutation is kept on the main thread
    // The worker thread only computes which indices should load/unload
    ProcessUnloadQueue();
    ProcessLoadQueue();
}

void AsyncLoadController::DebugUI()
{
    if (!ImGui::CollapsingHeader("AsyncLoading", ImGuiTreeNodeFlags_DefaultOpen))
    {
        return;
    }

    ImGui::DragFloat("Load Radius", &mLoadRadius, 1.0f, 1.0f, 200.0f, "%.0f");

    int loaded = 0;
    int loadQueued = 0;
    int unloadQueued = 0;
    for (const Cell& cell : mCells)
    {
        if (cell.state == CellState::Loaded)
        {
            ++loaded;
        }
        else if (cell.state == CellState::LoadingQueued)
        {
            ++loadQueued;
        }
        else if (cell.state == CellState::UnloadingQueued)
        {
            ++unloadQueued;
        }
    }

    ImGui::Text("Loaded: %d", loaded);
    ImGui::Text("PendingLoad: %d", loadQueued);
    ImGui::Text("PendingUnload: %d", unloadQueued);

    if (ImGui::Button("Unload All"))
    {
        for (Cell& cell : mCells)
        {
            if (cell.state == CellState::Loaded || cell.state == CellState::LoadingQueued)
            {
                cell.state = CellState::UnloadingQueued;
            }
        }
    }
}

void AsyncLoadController::BuildCells()
{
    mCells.clear();
    mCells.reserve(static_cast<size_t>(kGridCountX * kGridCountZ * 2));

    for (int z = 0; z < kGridCountZ; ++z)
    {
        for (int x = 0; x < kGridCountX; ++x)
        {
            Cell cactus;
            cactus.name = "Async_Cactus_" + std::to_string(z) + "_" + std::to_string(x);
            cactus.templatePath = kCactusTemplate;
            cactus.persistedTransform.position =
            {
                kCactusStartX + (static_cast<float>(x) * kGridSpacing),
                0.0f,
                kGridStartZ + (static_cast<float>(z) * kGridSpacing)
            };
            mCells.push_back(cactus);

            Cell salad;
            salad.name = "Async_Salad_" + std::to_string(z) + "_" + std::to_string(x);
            salad.templatePath = kSaladTemplate;
            salad.persistedTransform.position =
            {
                kSaladStartX + (static_cast<float>(x) * kGridSpacing),
                0.0f,
                kGridStartZ + (static_cast<float>(z) * kGridSpacing)
            };
            mCells.push_back(salad);
        }
    }
}

void AsyncLoadController::CapturePersistedTransforms()
{
    for (Cell& cell : mCells)
    {
        if (cell.state != CellState::Loaded || cell.gameObject == nullptr)
        {
            continue;
        }

        if (mGridPlacementController->IsHoldingObject(cell.gameObject))
        {
            continue;
        }

        TransformComponent* transformComponent = cell.gameObject->GetComponent<TransformComponent>();
        if (transformComponent == nullptr)
        {
            continue;
        }

        cell.persistedTransform.position = transformComponent->position;
        cell.persistedTransform.rotation = transformComponent->rotation;
        cell.persistedTransform.scale = transformComponent->scale;
    }
}

void AsyncLoadController::SubmitWorkerRequest()
{
    const CameraService* cameraService = mGameWorld->GetService<CameraService>();
    if (cameraService == nullptr)
    {
        return;
    }

    WorkerRequest request;
    request.cameraPosition = cameraService->GetMain().GetPosition();
    request.loadRadius = mLoadRadius;
    request.cellSnapshots.reserve(mCells.size());

    for (const Cell& cell : mCells)
    {
        request.cellSnapshots.push_back({ cell.persistedTransform.position, cell.state });
    }

    {
        std::lock_guard<std::mutex> lock(mRequestMutex);
        mPendingRequest = std::move(request);
        mHasPendingRequest = true;
    }
    mRequestCv.notify_one();
}

void AsyncLoadController::ApplyWorkerResult()
{
    WorkerResult result;
    {
        std::lock_guard<std::mutex> lock(mResultMutex);
        if (!mHasPendingResult)
        {
            return;
        }

        result = std::move(mPendingResult);
        mHasPendingResult = false;
    }

    for (size_t index : result.loadIndices)
    {
        if (index >= mCells.size())
        {
            continue;
        }

        Cell& cell = mCells[index];
        if (cell.state == CellState::Unloaded || cell.state == CellState::UnloadingQueued)
        {
            cell.state = CellState::LoadingQueued;
        }
    }

    for (size_t index : result.unloadIndices)
    {
        if (index >= mCells.size())
        {
            continue;
        }

        Cell& cell = mCells[index];
        if (cell.state == CellState::Loaded || cell.state == CellState::LoadingQueued)
        {
            cell.state = CellState::UnloadingQueued;
        }
    }
}

void AsyncLoadController::ProcessUnloadQueue()
{
    int remaining = mMaxUnloadsPerFrame;

    for (Cell& cell : mCells)
    {
        if (remaining <= 0)
        {
            return;
        }

        if (cell.state != CellState::UnloadingQueued || cell.gameObject == nullptr)
        {
            continue;
        }

        if (mGridPlacementController->IsHoldingObject(cell.gameObject))
        {
            continue;
        }

        if (TransformComponent* transformComponent = cell.gameObject->GetComponent<TransformComponent>())
        {
            cell.persistedTransform.position = transformComponent->position;
            cell.persistedTransform.rotation = transformComponent->rotation;
            cell.persistedTransform.scale = transformComponent->scale;
        }

        mGridPlacementController->UnregisterPickableFurniture(*cell.gameObject);

        // Engine game-object creation/destruction is not thread-safe in this project
        // We intentionally call DestroyGameObject on the main thread only
        mGameWorld->DestroyGameObject(cell.handle);

        cell.gameObject = nullptr;
        cell.state = CellState::Unloaded;
        --remaining;
    }
}

void AsyncLoadController::ProcessLoadQueue()
{
    int remaining = mMaxLoadsPerFrame;

    for (Cell& cell : mCells)
    {
        if (remaining <= 0)
        {
            return;
        }

        if (cell.state != CellState::LoadingQueued)
        {
            continue;
        }

        // Engine game-object creation/destruction is not thread-safe in this project
        // We intentionally call CreateGameObject on the main thread only
        GameObject* gameObject = mGameWorld->CreateGameObject(cell.name, cell.templatePath);
        if (gameObject == nullptr)
        {
            cell.state = CellState::Unloaded;
            continue;
        }

        if (TransformComponent* transformComponent = gameObject->GetComponent<TransformComponent>())
        {
            transformComponent->position = cell.persistedTransform.position;
            transformComponent->rotation = cell.persistedTransform.rotation;
            transformComponent->scale = cell.persistedTransform.scale;
        }

        gameObject->Initialize();
        mGridPlacementController->RegisterPickableFurniture(*gameObject);

        cell.gameObject = gameObject;
        cell.handle = gameObject->GetHandle();
        cell.state = CellState::Loaded;
        --remaining;
    }
}

void AsyncLoadController::StartWorkerThread()
{
    mWorkerRunning = true;
    mWorkerThread = std::thread(&AsyncLoadController::WorkerMain, this);
}

void AsyncLoadController::StopWorkerThread()
{
    mWorkerRunning = false;
    mRequestCv.notify_one();

    if (mWorkerThread.joinable())
    {
        mWorkerThread.join();
    }

    {
        std::lock_guard<std::mutex> lock(mRequestMutex);
        mHasPendingRequest = false;
        mPendingRequest = {};
    }

    {
        std::lock_guard<std::mutex> lock(mResultMutex);
        mHasPendingResult = false;
        mPendingResult = {};
    }
}

void AsyncLoadController::WorkerMain()
{
    while (mWorkerRunning)
    {
        WorkerRequest request;

        {
            std::unique_lock<std::mutex> lock(mRequestMutex);
            mRequestCv.wait(lock, [this]()
                {
                    return !mWorkerRunning || mHasPendingRequest;
                });

            if (!mWorkerRunning)
            {
                return;
            }

            request = std::move(mPendingRequest);
            mHasPendingRequest = false;
        }

        WorkerResult result;
        const float loadRadiusSqr = request.loadRadius * request.loadRadius;

        for (size_t index = 0; index < request.cellSnapshots.size(); ++index)
        {
            const CellSnapshot& cell = request.cellSnapshots[index];
            const float distanceSqr = ComputeDistanceSqrXZ(request.cameraPosition, cell.position);
            const bool shouldBeLoaded = distanceSqr <= loadRadiusSqr;

            if (shouldBeLoaded)
            {
                if (cell.state == CellState::Unloaded || cell.state == CellState::UnloadingQueued)
                {
                    result.loadIndices.push_back(index);
                }
            }
            else
            {
                if (cell.state == CellState::Loaded || cell.state == CellState::LoadingQueued)
                {
                    result.unloadIndices.push_back(index);
                }
            }
        }

        {
            std::lock_guard<std::mutex> lock(mResultMutex);
            mPendingResult = std::move(result);
            mHasPendingResult = true;
        }
    }
}

float AsyncLoadController::ComputeDistanceSqrXZ(const Vector3& a, const Vector3& b)
{
    const float dx = a.x - b.x;
    const float dz = a.z - b.z;
    return (dx * dx) + (dz * dz);
}
