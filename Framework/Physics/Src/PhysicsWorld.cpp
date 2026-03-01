#include "Precompiled.h"
#include "PhysicsWorld.h"
#include "PhysicsObject.h"

using namespace Engine3D;
using namespace Engine3D::Physics;

namespace
{
    std::unique_ptr<PhysicsWorld> sPhysicsWorld;
}

void PhysicsWorld::StaticInitialize(const Settings& settings)
{
    ASSERT(sPhysicsWorld == nullptr, "PhyscsWorld : is already initialized");
    sPhysicsWorld = std::make_unique<PhysicsWorld>();
    sPhysicsWorld->Initialize(settings);
}

void PhysicsWorld::StaticTerminate()
{
    if(sPhysicsWorld != nullptr)
    {
        sPhysicsWorld->Terminate();
        sPhysicsWorld.reset();
    }
}

PhysicsWorld* PhysicsWorld::Get()
{
    ASSERT(sPhysicsWorld != nullptr, "PhysicsWorld : is not initialized");
    return sPhysicsWorld.get();
}

PhysicsWorld::~PhysicsWorld()
{
    ASSERT(mDynamicsWorld == nullptr, "PhysicsWorld : must be terminated");
}

void PhysicsWorld::Initialize(const Settings& settings)
{
    mSettings = settings;
    mInterface = new btDbvtBroadphase();
    mSolver = new btSequentialImpulseConstraintSolver();

#ifdef USE_SOFT_BODY

    mCollisionConfiguration = new btSoftBodyRigidBodyCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollisionConfiguration);
    mDynamicsWorld = new btSoftRigidDynamicsWorld(mDispatcher, mInterface, mSolver, mCollisionConfiguration);

#else
    mCollissionConfiguration = new btDefaultCollisionConfiguration();
    mDispatcher = new btCollisionDispatcher(mCollissionConfiguration);
    mDynamicsWorld = new btDiscreteDynamicsWorld(mDispatcher, mInterface, mSolver, mCollissionConfiguration);
#endif

    mDynamicsWorld->setGravity(TobtVector3(mSettings.gravity));
    mDynamicsWorld->setDebugDrawer(&mPhysicsDebugDraw);
}

void PhysicsWorld::Terminate()
{
    SafeDelete(mDynamicsWorld);
    SafeDelete(mDispatcher);
    SafeDelete(mCollisionConfiguration);
    SafeDelete(mSolver);
    SafeDelete(mInterface);
}

void PhysicsWorld::Update(float deltaTime)
{
    mDynamicsWorld->stepSimulation(deltaTime, mSettings.simulationSteps, mSettings.fixedTimeStep);
    for (PhysicsObject* obj : mPhysicsObjects)
    {
        obj->SyncWithGraphics();
    }
}

void PhysicsWorld::DebugUI()
{
    if (ImGui::CollapsingHeader("Physics", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::DragFloat3("Gravity", &mSettings.gravity.x, 0.1f))
        {
            mDynamicsWorld->setGravity(TobtVector3(mSettings.gravity));
        }
        ImGui::Checkbox("Debug Draw", &mDebugDraw);

        if (mDebugDraw)
        {
            ImGui::Indent();
            int debugMode = mPhysicsDebugDraw.getDebugMode();
            bool drawWireframe = (debugMode & btIDebugDraw::DBG_DrawWireframe);

            if (ImGui::Checkbox("Draw Wireframe", &drawWireframe))
            {
                debugMode = (drawWireframe) ? debugMode | btIDebugDraw::DBG_DrawWireframe : debugMode & ~btIDebugDraw::DBG_DrawWireframe;

            }
            bool drawAABB = (debugMode & btIDebugDraw::DBG_DrawAabb);

            if (ImGui::Checkbox("Draw AABB", &drawAABB))
            {
                debugMode = (drawAABB) ? debugMode | btIDebugDraw::DBG_DrawAabb : debugMode & ~btIDebugDraw::DBG_DrawAabb;
            }
            bool drawContactPoints = (debugMode & btIDebugDraw::DBG_DrawContactPoints);

            if (ImGui::Checkbox("Draw Contact Points", &drawContactPoints))
            {
                debugMode = (drawContactPoints) ? debugMode | btIDebugDraw::DBG_DrawContactPoints : debugMode & ~btIDebugDraw::DBG_DrawContactPoints;
            }

            mPhysicsDebugDraw.setDebugMode(debugMode);
            mDynamicsWorld->debugDrawWorld();
            ImGui::Unindent();
        }
    }
}

void PhysicsWorld::SetGravity(const Math::Vector3& gravity)
{
    mSettings.gravity = gravity;
    mDynamicsWorld->setGravity(TobtVector3(gravity));
}

void PhysicsWorld::Register(PhysicsObject* physicsObject)
{
    auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
    // if iter is the end, it is NOT in the list, register means we wnt to add it, so it is safe to add now
    if (iter == mPhysicsObjects.end())
    {
        mPhysicsObjects.push_back(physicsObject);

#ifdef USE_SOFT_BODY
        if (physicsObject->GetSoftBody() != nullptr)
        {
            mDynamicsWorld->addSoftBody(physicsObject->GetSoftBody());
        }
#endif

        if (physicsObject->GetRigidBody() != nullptr)
        {
            mDynamicsWorld->addRigidBody(physicsObject->GetRigidBody());
        }
    }
}

void PhysicsWorld::Unregister(PhysicsObject* physicsObject)
{
    auto iter = std::find(mPhysicsObjects.begin(), mPhysicsObjects.end(), physicsObject);
    // if iter is NOT the end, it is IS in the list, register means we wnt to remove it, we found it so we can remove now
    if (iter != mPhysicsObjects.end())
    {

#ifdef USE_SOFT_BODY
        if (physicsObject->GetSoftBody() != nullptr)
        {
            mDynamicsWorld->removeSoftBody(physicsObject->GetSoftBody());
        }
#endif

        if (physicsObject->GetRigidBody() != nullptr)
        {
            mDynamicsWorld->removeRigidBody(physicsObject->GetRigidBody());
        }
        mPhysicsObjects.erase(iter);
    }
}
