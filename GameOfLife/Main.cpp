#include <stdio.h>
#include "imgui.h"
#include "Window.h"
#include "LayerManager.h"
#include "Layers/UILayer.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


static bool isRunning = true;
static float deltaTime = 0.0f;

void run(Window* window, LayerManager* layerManager)
{    
    layerManager->insertNewLayer(std::make_unique<UILayer>(layerManager));
    float lastTime = 0.0f;
    while (isRunning) {
        float time = static_cast<float>(glfwGetTime());
        // handle timestep:
        deltaTime = time - lastTime;
        lastTime = time;

        layerManager->updateLayers(/*deltaTime*/);

        
        layerManager->renderLayers();

        glClearColor(0.2f,
                     0.4f,
                     0.2f ,
                     1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        if (glfwWindowShouldClose(window->window())) {
            isRunning = false;
        }

        window->onUpdate();
    }
}

int main(int argc, char* argv[])
{
    for (int i = 0; i < argc; ++i) {
        printf("%s ", argv[i]);
        if (i == argc - 1) { printf("\n"); }
    }

    std::shared_ptr<Window> window = std::make_shared<Window>();
    std::unique_ptr<LayerManager> layerManager = std::make_unique<LayerManager>(window);
    
    run(window.get(), layerManager.get());

    return 0;
}
