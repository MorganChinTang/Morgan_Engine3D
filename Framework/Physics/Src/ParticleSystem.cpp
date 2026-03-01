#include "Precompiled.h"
#include "ParticleSystem.h"

using namespace Engine3D;
using namespace Engine3D::Physics;
using namespace Engine3D::Graphics;

void ParticleSystem::Initialize(const ParticleSystemInfo& info) {
    mInfo = info;
    mNextAvailableParticleIndex = 0;
    mNextSpawnTime = info.delay;
    mLifeTime = info.lifeTime;

    InitializeParticles(info.maxParticles);
}

void ParticleSystem::Terminate()
{
    for (auto& particle : mParticles)
    {
        particle->Terminate();
        particle.reset();
    }
    mParticles.clear();
    TextureManager::Get()->ReleaseTexture(mInfo.textureId);
}

void ParticleSystem::Update(float deltaTime) {
    if (IsActive())
    {
        mLifeTime -= deltaTime;
        mNextSpawnTime -= deltaTime;
        if (mNextSpawnTime <= 0.0f && mLifeTime > 0.0f)
        {
            SpawnParticles();
        }
        for (auto& particle : mParticles)
        {
            particle->Update(deltaTime);
        }
    }
}

bool ParticleSystem::IsActive()
{
    if (mLifeTime > 0.0f)
    {
        return true;
    }
    for (auto& particle : mParticles)
    {
        if (particle->IsActive())
        {
            return true;
        }
    }
    return false;
}

void ParticleSystem::DebugUI()
{
    ImGui::PushID(this);
    if (ImGui::CollapsingHeader("ParticleSystem"))
    {
        ImGui::DragFloat3("Position", &mInfo.spawnPosition.x);
        if (ImGui::DragFloat("Direction", &mInfo.spawnDirection.x))
        {
            mInfo.spawnDirection = Math::Normalize(mInfo.spawnDirection);
        }
        ImGui::DragIntRange2("PerEmit", &mInfo.particlesPerEmit.min, &mInfo.particlesPerEmit.max);
        ImGui::DragFloatRange2("TimeBetweenEmit", &mInfo.timeBetweenEmit.min, &mInfo.timeBetweenEmit.max);
        ImGui::DragFloatRange2("SpawnAngle", &mInfo.spawnSpeed.min, &mInfo.spawnSpeed.max);
        ImGui::DragFloatRange2("SpawnSpeed", &mInfo.spawnSpeed.min, &mInfo.spawnSpeed.max);
        ImGui::DragFloatRange2("ParticleLifeTime", &mInfo.particleLifeTime.min, &mInfo.particleLifeTime.max);
        ImGui::DragFloat3("StartSaleMin", &mInfo.startScale.min.x);
        ImGui::DragFloat3("StartSaleMax", &mInfo.startScale.max.x);
        ImGui::DragFloat3("EndSaleMin", &mInfo.endScale.min.x);
        ImGui::DragFloat3("EndSaleMax", &mInfo.endScale.max.x);
        ImGui::ColorEdit4("StartColorMin", &mInfo.startColor.min.r);
        ImGui::ColorEdit4("StartColorMax", &mInfo.startColor.max.r);
        ImGui::ColorEdit4("EndColorMin", &mInfo.endColor.min.r);
        ImGui::ColorEdit4("EndColorMax", &mInfo.endColor.max.r);
    }
    ImGui::PopID();
}

void ParticleSystem::SetPosition(const Math::Vector3& position)
{
    mInfo.spawnPosition = position;
}

void ParticleSystem::SpawnParticles()
{
    int numParticles = mInfo.particlesPerEmit.GetRandomInc();
    for (int i = 0; i < numParticles; ++i)
    {
        SpawnSingleParticle();
    }
    mNextSpawnTime = mInfo.timeBetweenEmit.GetRandom();
}

void ParticleSystem::Render(Graphics::ParticleSystemEffect& effect)
{
    if (!IsActive())
    {
        return;
    }

    effect.SetTexture(mInfo.textureId);

    for (auto& particle : mParticles)
    {
        if (particle->IsActive())
        {
            effect.Render(particle->GetTransform(), particle->GetColor());
        }
    }
}

void ParticleSystem::InitializeParticles(uint32_t maxParticles)
{
    mParticles.resize(maxParticles);
    for (uint32_t i = 0; i < maxParticles; ++i)
    {
        mParticles[i] = std::make_unique<Particle>();
        mParticles[i]->Initialize();
    }
}

void ParticleSystem::SpawnSingleParticle()
{
    Particle* particle = mParticles[mNextAvailableParticleIndex].get();
    mNextAvailableParticleIndex = (mNextAvailableParticleIndex + 1) % mParticles.size();

    const bool isUp = (Math::Abs(Math::Dot(mInfo.spawnDirection, Math::Vector3::YAxis))) > 0.9999f;
    Math::Vector3 r = (isUp) ? Math::Vector3::XAxis :
        Math::Normalize(Math::Cross(Math::Vector3::YAxis, mInfo.spawnDirection));
    Math::Vector3 u = Math::Normalize(Math::Cross(mInfo.spawnDirection, r));

    float rotAngle = mInfo.spawnAngle.GetRandom() * Math::Constants::DegToRad;
    Math::Matrix4 mathRotRight = Math::Matrix4::RotationAxis(r, rotAngle);

    //get a random angle, rotate the look around the up axis 
    rotAngle = mInfo.spawnAngle.GetRandom() * Math::Constants::DegToRad;
    Math::Matrix4 mathRotUp = Math::Matrix4::RotationAxis(u, rotAngle);

    // combine rotations to get a final rotation matrix
    Math::Matrix4 matRotation = mathRotRight * mathRotUp;
    Math::Vector3 spawnDirection = Math::TransformNormal(mInfo.spawnDirection, matRotation);

    const float speed = mInfo.spawnSpeed.GetRandom();
    ParticleInfo info;
    info.lifeTime = mInfo.particleLifeTime.GetRandom();
    info.startColor = mInfo.startColor.GetRandom();
    info.endColor = mInfo.endColor.GetRandom();
    info.startScale = mInfo.startScale.GetRandom();
    info.endScale = mInfo.endScale.GetRandom();
    info.position = mInfo.spawnPosition;
    info.velocity = spawnDirection * speed;
    particle->Activate(info);
}