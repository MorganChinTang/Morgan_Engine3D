#pragma once
#include "NetworkEvents.h"

namespace Engine3D::Network
{
    // get the position/velocity
    // info is in the past (latency)
    // try to predict next update location
    // lerp there (or stay on track until next update)
    // get periodic updates
    class NetworkController
    {
    public:
        NetworkController() = default;
        ~NetworkController() = default;

        void SetPosition(const Math::Vector3& position);
        bool TryGetNewPosition(Math::Vector3& newPosition);

        void SetInput(const EventInput& input);
        bool TryGetDirtyInput(EventInput& input);
        const EventInput& GetInput() const;

        void SetLocalTransform(const Graphics::Transform* transform);
        const Graphics::Transform* GetLocalTransform() const;

        void SetRigidBody(const Physics::RigidBody* rigidBody);
        const Physics::RigidBody* GetRigidBody() const;

        void Update(float deltaTime);
        void SetTargetTransform(const Graphics::Transform& targetTransform, float timeStamp);
        const Graphics::Transform& GetSimTransform() const;

    private:
        bool mNewPosition = false;
        Math::Vector3 mPosition;

        bool mInputIsDirty = false;
        EventInput mInput;

        float mLastTimeStamp = 0.0;
        float mCurrentTime = 0.0;
        Graphics::Transform mTargetSimTransform;

        // sim transform is current simulated, predicted position based on network events
        Graphics::Transform mSimTransform;

        // local transform is just what the local object is position
        const Graphics::Transform* mLocalTransform = nullptr;

        // rigid body needed for velocities
        const Physics::RigidBody* mRigidBody = nullptr;
    };
}