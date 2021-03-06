#include "Core.h"

#include "../Components/Transform.h"
#include "../Components/Camera.h"

#include "../Systems/CameraHandler.h"

#include "Subsystems/ECS/ECSManager.h"
#include "Subsystems/Window/Window.h"
#include "Subsystems/Renderer/Renderer.h"

#include <random>

ECSManager  g_ECSManager;
Window      g_Window;
Renderer    g_Renderer;

int Core::Run()
{
    RegisterAllComponents();


    // Camera system initialization
    const auto cameraSystem = g_ECSManager.registerSystem<CameraHandler>();
    const Signature cameraSignature = [&](){
        Signature s;
        s.set(g_ECSManager.getComponentType<Transform>());
        s.set(g_ECSManager.getComponentType<Camera>());
        return s;
    }();
    g_ECSManager.setSystemSignature<CameraHandler>(cameraSignature);

    Entity mainEntity = g_ECSManager.createEntity();
    g_ECSManager.addComponent
    (
        mainEntity,
        Transform
        {
            .position = {2.221714f, 2.839314f, -6.113855f},
            .rotation = glm::vec3(0, 0, 0),
            .scale = glm::vec3(1.0f, 1.0f, 1.0f)
        }
    );

    g_ECSManager.addComponent
    (
        mainEntity,
        Camera
        {
            .FOV = 60.0f,
            .yaw = 111.5f,
            .pitch = -19.5f,
            .speed = 2.0f,
        }
    );

    float dt = 0.0f;

    while (!g_Window.windowShouldClose())
    {
        const auto startTime = std::chrono::high_resolution_clock::now();

        g_Window.pollEvents();

        cameraSystem->Update(dt);

        g_Renderer.drawFrame();

        const auto stopTime = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<float, std::chrono::seconds::period>(stopTime - startTime).count();
        //INFO("FPS: " << 1.0/dt);
    }

    g_Renderer.waitIdle();

    return 0;  
}

void Core::RegisterAllComponents() const
{
    g_ECSManager.registerComponent<Transform>();
    g_ECSManager.registerComponent<Camera>();
}

