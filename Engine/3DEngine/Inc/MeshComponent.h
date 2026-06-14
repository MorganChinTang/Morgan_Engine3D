#pragma once

#include "RenderObjectComponent.h"

namespace Engine3D
{

    class MeshComponent : public RenderObjectComponent
    {
    public:
        SET_TYPE_ID(ComponentId::Mesh);

        void Deserialize(const rapidjson::Value& value) override;
        void Serialize(rapidjson::Document& doc, rapidjson::Value& value, const rapidjson::Value& originalValue) override;
        const Graphics::Model& GetModel() const override;

    private:
        Graphics::Model mMeshModel;

    };
}