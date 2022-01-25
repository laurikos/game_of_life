#include <stdio.h>
#include "Application.h"
// #include "imgui.h"
// #include "Window.h"
// #include "LayerManager.h"
// #include "Layers/UILayer.h"
// #include "TemporaryRenderer.h"
// #include "Camera.h"
// #include "Shader.h"

// #include <glad/glad.h>
// #include <GLFW/glfw3.h>

int main(int argc, char* argv[])
{
    printf("\n\n========= APPLICATION STARTS =========\n\n");
    for (int i = 0; i < argc; ++i) {
        printf("%s ", argv[i]);
        if (i == argc - 1) { printf("\n"); }
    }

    std::unique_ptr<Application> app = std::make_unique<Application>();
    app->run();
    
    printf("\n\n========= EXITING APPLICATION =========\n\n");
    
    return 0;
}

// static bool isRunning = true;
// static float deltaTime = 0.0f;

// struct RendererData
// {
//     std::uint32_t vertexArray;
//     std::uint32_t vertexBuffer;
//     std::uint32_t indexBuffer;
//     std::unique_ptr<Shader> shader;

//     RendererData() {}
// };

// void run(Window* window, LayerManager* layerManager)
// {
//     auto uiLayer = layerManager->insertNewLayer(std::make_unique<UILayer>(layerManager));
//     assert(uiLayer->isImGuiLayer() && "should've been UI layer");

//     std::unique_ptr<RendererData> rendererData = std::make_unique<RendererData>();
    
//     glGenVertexArrays(1, &rendererData->vertexArray);
//     glBindVertexArray(rendererData->vertexArray);

//     glGenBuffers(1, &rendererData->vertexBuffer);
//     glBindBuffer(GL_ARRAY_BUFFER, rendererData->vertexBuffer);

//     float vertices[3 * 3] = {
//         -0.5f, -0.5f, 0.0f,
//          0.5f, -0.5f, 0.0f,
//          0.0f,  0.5f, 0.0f
//     };

//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     glEnableVertexAttribArray(0);
//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

//     glGenBuffers(1, &rendererData->indexBuffer);
//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, rendererData->indexBuffer);

//     std::uint32_t indices[3] = { 0, 1, 2 };
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//     std::string vertexShaderSource = R"(
// #version 330 core

// layout(location = 0) in vec3 a_Position;

// void main()
// {
//     gl_Position = vec4(a_Position, 1.0);
// }

// )";

//     std::string fragmentShaderSource = R"(
// #version 330 core

// layout(location = 0) out vec4 color;

// void main()
// {
//     color = vec4(0.22, 0.44, 0.33, 1.0);
// }

// )";
    
//     rendererData->shader = std::make_unique<Shader>(vertexShaderSource, fragmentShaderSource);
    
//     float lastTime = 0.0f;
//     while (isRunning) {
//         float time = static_cast<float>(glfwGetTime());
//         // handle timestep:
//         deltaTime = time - lastTime;
//         lastTime = time;

//         TemporaryRenderer::setClearColor();
//         TemporaryRenderer::clear();

//         rendererData->shader->bind();
//         glBindVertexArray(rendererData->vertexArray);
//         glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, nullptr);

//         if (glfwWindowShouldClose(window->window())) {
//             isRunning = false;
//         }
        
//         layerManager->updateLayers(deltaTime);
//         layerManager->renderLayers();
//         window->onUpdate();
//     }
// }

// int main(int argc, char* argv[])
// {
//     for (int i = 0; i < argc; ++i) {
//         printf("%s ", argv[i]);
//         if (i == argc - 1) { printf("\n"); }
//     }

//     std::shared_ptr<Window> window = std::make_shared<Window>();
//     std::unique_ptr<LayerManager> layerManager = std::make_unique<LayerManager>(window);

//     run(window.get(), layerManager.get());

//     Camera* cam = new Camera(1.0f, 1.0f, 1.0f, 1.0f);
//     delete cam;
    
//     return 0;
// }
