#include <3DEngine/Inc/3DEngine.h>

class MainState : public Engine3D::AppState
{
    void Initialize()
    {
        LOG("MainState: Initialize");
        mLifeTime = 2.0f;
    }
    void Terminate()
    {
        LOG("MainState: Terminate");
    }
    void Update(float deltaTime) override
    {
        mLifeTime -= std::max(deltaTime, 0.01f);
        if (mLifeTime <= 0.0f)
        {
            Engine3D::MainApp().ChangeState("GameState");
        }
    }
private:
    float mLifeTime = 0.0f;
};

class GameState : public Engine3D::AppState
{
public:
    void Initialize() override
    {
        LOG("GameState: Initialize");
        mLifeTime = 2.0f;
    }
    void Terminate() override
    {
        LOG("GameState: Terminate");
    }
    void Update(float deltaTime) override
    {
        mLifeTime -= std::max(deltaTime, 0.01f);
        if (mLifeTime <= 0.0f)
        {
            Engine3D::MainApp().ChangeState("MainState");
        }
    }
private:
    float mLifeTime = 0.0f;
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int) 
{
    Engine3D::AppConfig config;
    Engine3D::App& myApp = Engine3D::MainApp();
    myApp.AddState<MainState>("MainState");
    myApp.AddState<GameState>("GameState");
    myApp.Run(config);

    return 0;
}