#include "Precompiled.h"
#include "AnimationUtil.h"

#include "Color.h"
#include "SimpleDraw.h"

using namespace Engine3D;
using namespace Engine3D::Graphics;

// empty namespace for global functions isolated to the cpp file
namespace
{
    void ComputeBoneTransformsRecursive(const Bone* bone, AnimationUtil::BoneTransforms& boneTransforms, const Animator* animator)
    {
        if(bone!=nullptr)
        {
            if (animator == nullptr || !animator->GetToParentTransform(bone, boneTransforms[bone->index]))
            {
                // set the bone transform to the array of matrices
                boneTransforms[bone->index] = bone->toParentTransform;
            }
            // if there is a parent, apply the parents transform as wel
            if (bone->parent != nullptr)
            {
                boneTransforms[bone->index] = boneTransforms[bone->index] * boneTransforms[bone->parentIndex];
            }
            // go through the chidren and apply their transforms
            for (const Bone* child : bone->children)
            {
                ComputeBoneTransformsRecursive(child, boneTransforms, animator);
            }
        }
    }
}

void AnimationUtil::ComputeBoneTransforms(ModelId modelId, BoneTransforms& boneTransforms, const Animator* animator)
{
    const Model* model = ModelManager::Get()->GetModel(modelId);
    if (model != nullptr&& model->skeleton != nullptr)
    {
        // resize to sync the number of bones with the matrices
        boneTransforms.resize(model->skeleton->bones.size());
        // generate the matrices
        ComputeBoneTransformsRecursive(model->skeleton->root, boneTransforms, animator);
    }
}

void AnimationUtil::DrawSkeleton(ModelId modelId, const BoneTransforms& boneTransforms)
{
    const Model* model = ModelManager::Get()-> GetModel(modelId);
    if (model != nullptr && model->skeleton != nullptr)
    {
        // iterate through the unique bone pointers
        for (const auto& bone : model->skeleton->bones)
        {
            if (bone->parent != nullptr)
            {
                // gets the bone and parent bone position
                const Math::Vector3 bonePos = Math::GetTranslation(boneTransforms[bone->index]);
                const Math::Vector3 parentPos = Math::GetTranslation(boneTransforms[bone->parentIndex]);
                // draws a line from the bone to its parent
                SimpleDraw::AddLine(bonePos, parentPos, Colors::LightPink);
                // draws a sphere at the joint
                SimpleDraw::AddSphere(10, 10, 0.03f, Colors::LightCoral, bonePos);
            }
            
        }
    }
}

void AnimationUtil::ApplyBoneOffset(ModelId modelId, BoneTransforms& boneTransforms)
{
    const Model* model = ModelManager::Get()->GetModel(modelId);
    if (model != nullptr && model->skeleton != nullptr)
    {
        for(auto& bone : model->skeleton->bones)
        {
            boneTransforms[bone->index] = bone->offsetTransform * boneTransforms[bone->index];
        }
    }
}
