#include "Precompiled.h"
#include "GameObjectFactory.h"
#include "GameObject.h"
#include "GameWorld.h"
#include "Component.h"
#include "TransformComponent.h"
#include "CameraComponent.h"
#include "FPSCameraComponent.h"
#include "MeshComponent.h"
#include "ModelComponent.h"
#include "AnimatorComponent.h"
#include "RigidbodyComponent.h"
#include "SoundEventComponent.h"
#include "SoundBankComponent.h"
#include "UITextComponent.h"
#include "UISpriteComponent.h"
#include "UIButtonComponent.h"
#include "PlayerControllerComponent.h"

using namespace Engine3D;

namespace
{

    CustomComponent TryMakeComponent;
    CustomComponent TryGetComponent;
    Component* AddComponent(const std::string& componentName, GameObject& gameObject)
    {
        Component* newComponent = nullptr;
        if (componentName == "TransformComponent")
        {
            newComponent = gameObject.AddComponent<TransformComponent>();
        }
        else if (componentName == "CameraComponent")
        {
            newComponent = gameObject.AddComponent<CameraComponent>();
        }
        else if (componentName == "FPSCameraComponent")
        {
            newComponent = gameObject.AddComponent<FPSCameraComponent>();
        }
        else if (componentName == "MeshComponent")
        {
            newComponent = gameObject.AddComponent<MeshComponent>();
        }
        else if (componentName == "ModelComponent")
        {
            newComponent = gameObject.AddComponent<ModelComponent>();
        }
        else if (componentName == "AnimatorComponent")
        {
            newComponent = gameObject.AddComponent<AnimatorComponent>();
        }
        else if (componentName == "RigidBodyComponent")
        {
            newComponent = gameObject.AddComponent<RigidBodyComponent>();
        }
        else if (componentName == "SoundEventComponent")
        {
            newComponent = gameObject.AddComponent<SoundEventComponent>();
        }
        else if (componentName == "SoundBankComponent")
        {
            newComponent = gameObject.AddComponent<SoundBankComponent>();
        }
        else if (componentName == "UITextComponent")
        {
            newComponent = gameObject.AddComponent<UITextComponent>();
        }
        else if (componentName == "UISpriteComponent")
        {
            newComponent = gameObject.AddComponent<UISpriteComponent>();
        }
        else if (componentName == "UIButtonComponent")
        {
            newComponent = gameObject.AddComponent<UIButtonComponent>();
        }
        else if (componentName == "PlayerControllerComponent")
        {
            newComponent = gameObject.AddComponent<PlayerControllerComponent>();
        }
        else
        {
            newComponent = TryMakeComponent(componentName, gameObject);
        }

        ASSERT(newComponent != nullptr, "GameObjectFactory: component type [%s] not found", componentName.c_str());
        return newComponent;
    }

}


Component* GetComponent(const std::string& componentName, GameObject& gameObject)
{
    Component* newComponent = nullptr;
    if (componentName == "TransformComponent")
    {
        newComponent = gameObject.GetComponent<TransformComponent>();
    }
    else if (componentName == "CameraComponent")
    {
        newComponent = gameObject.GetComponent<CameraComponent>();
    }
    else if (componentName == "FPSCameraComponent")
    {
        newComponent = gameObject.GetComponent<FPSCameraComponent>();
    }
    else if (componentName == "MeshComponent")
    {
        newComponent = gameObject.GetComponent<MeshComponent>();
    }
    else if (componentName == "ModelComponent")
    {
        newComponent = gameObject.GetComponent<ModelComponent>();
    }
    else if (componentName == "AnimatorComponent")
    {
        newComponent = gameObject.GetComponent<AnimatorComponent>();
    }
    else if (componentName == "RigidBodyComponent")
    {
        newComponent = gameObject.GetComponent<RigidBodyComponent>();
    }
    else if (componentName == "SoundEventComponent")
    {
        newComponent = gameObject.GetComponent<SoundEventComponent>();
    }
    else if (componentName == "SoundBankComponent")
    {
        newComponent = gameObject.GetComponent<SoundBankComponent>();
    }
    else if (componentName == "UITextComponent")
    {
        newComponent = gameObject.GetComponent<UITextComponent>();
    }
    else if (componentName == "UISpriteComponent")
    {
        newComponent = gameObject.GetComponent<UISpriteComponent>();
    }
    else if (componentName == "UIButtonComponent")
    {
        newComponent = gameObject.GetComponent<UIButtonComponent>();
    }
    else if (componentName == "PlayerControllerComponent")
    {
        newComponent = gameObject.GetComponent<PlayerControllerComponent>();
    }
    else
    {
        newComponent = TryGetComponent(componentName, gameObject);
    }

    ASSERT(newComponent != nullptr, "GameObjectFactory: component type [%s] not found", componentName.c_str());
    return newComponent;
}

void GameObjectFactory::SetCustomMake(CustomComponent callback)
{
    TryMakeComponent = callback;
}

void GameObjectFactory::SetCustomGet(CustomComponent callback)
{
    TryGetComponent = callback;
}

void GameObjectFactory::Make(const std::filesystem::path& templatePath, GameObject& gameObject, GameWorld& gameWorld)
{
    FILE* file = nullptr;
    auto err = fopen_s(&file, templatePath.u8string().c_str(), "r");
    ASSERT(err == 0, "GameObjectFactory: Failed to open file %s", templatePath.u8string().c_str());

    char readBuffer[65536];
    rapidjson::FileReadStream readStream(file, readBuffer, sizeof(readBuffer));
    rapidjson::Document doc;
    doc.ParseStream(readStream);
    fclose(file);

    ASSERT(!doc.HasParseError(), "GameObjectFactory: Failed to parse json file %s", templatePath.u8string().c_str());
    ASSERT(doc.IsObject(), "GameObjectFactory: Root json is not an object in file %s", templatePath.u8string().c_str());
    ASSERT(doc.HasMember("Components") && doc["Components"].IsObject(), "GameObjectFactory: Missing or invalid Components object in file %s", templatePath.u8string().c_str());

    auto components = doc["Components"].GetObj();
    for (auto& component : components)
    {
        // read data
        Component* newComponent = AddComponent(component.name.GetString(), gameObject);
        if(newComponent!=nullptr)
        {
            // apply the json value data
            newComponent->Deserialize(component.value);
        }
    }

    if (doc.HasMember("Children"))
    {
        ASSERT(doc["Children"].IsObject(), "GameObjectFactory: Children must be an object in file %s", templatePath.u8string().c_str());
        auto children = doc["Children"].GetObj();
        for (auto& child : children)
        {
            ASSERT(child.value.IsObject(), "GameObjectFactory: Child entry [%s] must be an object in file %s", child.name.GetString(), templatePath.u8string().c_str());
            ASSERT(child.value.HasMember("Template") && child.value["Template"].IsString(), "GameObjectFactory: Child entry [%s] missing Template string in file %s", child.name.GetString(), templatePath.u8string().c_str());

            std::string name = child.name.GetString();
            std::filesystem::path childTemplate = child.value["Template"].GetString();
            GameObject* childGO = gameWorld.CreateGameObject(name, childTemplate);

            OverrideDeserialize(child.value, *childGO);
            gameObject.AddChild(childGO);
            childGO->SetParent(&gameObject);

        }
    }
}

void GameObjectFactory::OverrideDeserialize(const rapidjson::Value& value, GameObject& gameObject)
{
    ASSERT(value.IsObject(), "GameObjectFactory: OverrideDeserialize value must be an object");
    if (value.HasMember("Components"))
    {
        ASSERT(value["Components"].IsObject(), "GameObjectFactory: Override Components must be an object");
        auto components = value["Components"].GetObj();
        for (auto& component : components)
        {
            Component* ownedComponent = GetComponent(component.name.GetString(), gameObject);
            if (ownedComponent != nullptr)
            {
                ownedComponent->Deserialize(component.value);
            }
        }
    }
}
