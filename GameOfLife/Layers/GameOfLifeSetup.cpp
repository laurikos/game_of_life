#include "GameOfLifeSetup.h"
#include "../LayerManager.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <vector>

struct GameOfLifeSetup::PImpl {
    GameOfLifeSetup& m_parent;
    LayerManager* m_manager;

    bool m_isInitialized;
    bool m_isGameAreaSizeSet;
    bool m_isInitialCellStateSet;    
    
    std::uint32_t m_gameAreaLenX;
    std::uint32_t m_gameAreaLenY;
    std::vector<std::vector<std::uint32_t>> m_initialCellState;

    
    
    PImpl(GameOfLifeSetup& parent, LayerManager* manager);
    ~PImpl();

    void init();
    void onUpdate(float deltaTime);
    void onRender();

    void startSceneUI();
    void endSceneUI();
};

GameOfLifeSetup::PImpl::PImpl(GameOfLifeSetup& parent, LayerManager* manager)
    : m_parent(parent),
      m_manager(manager)
{

}

GameOfLifeSetup::PImpl::~PImpl()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void GameOfLifeSetup::PImpl::init()
{
    /*
     * SET UP IMGUI
     */
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();

    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();

    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags/* & ImGuiConfigFlags_ViewportsEnable*/)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    auto window = m_manager->getWindow();
    
    const char* glslVersion = "#version 330";
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glslVersion);

    /*
     * SET UP GAME:
     */
    m_isInitialized = false;
    m_isGameAreaSizeSet = false;
    m_isInitialCellStateSet = false;
}

void GameOfLifeSetup::PImpl::onUpdate(float deltaTime)
{

}

void GameOfLifeSetup::PImpl::onRender()
{
    static bool allDone = false;
    if (allDone) {
        return;
    }
    if (m_isInitialized && m_isGameAreaSizeSet && m_isInitialCellStateSet)
    {
        allDone = true;
        m_manager->settingUpDone(m_initialCellState);
    }
    
    if (!m_isInitialized) {

        if (!m_isGameAreaSizeSet) {
            ImGui::Begin("Game area size");
            ImGui::Text("Set width and height for the game of life area");
            ImGui::Text("Minimum values: 2 -- Maximum 10 000 quads [e.g. 100 x 100]");

            static int tmpy = 70;
            static int tmpx = 140;

            static int x = 70;
            ImGui::InputInt("Width", &x);

            static int y = 140;
            ImGui::InputInt("Height", &y);

            
            bool done = false;
            static int clicked = 0;
            if ((tmpx != x) || (tmpy != y)) {
                clicked = 0;
            }
            if (ImGui::Button("OK", ImVec2(40, 40))) {
                clicked++;
            }
            if (clicked & 1) {
                if ((y >= 2 && x >= 2) && (x * y <= 10000)) {
                    done = true;
                } else {
                    tmpx = x;
                    tmpy = y;
                    ImGui::SameLine();
                    ImGui::Text("Invalid values");
                }
            }

            if (done) {
                m_gameAreaLenX = x;
                m_gameAreaLenY = y;
                m_initialCellState = std::vector(
                    m_gameAreaLenY, std::vector<std::uint32_t>(m_gameAreaLenX));
                m_isGameAreaSizeSet = true;
            }
            
            ImGui::End();
        }
        else if (!m_isInitialCellStateSet) {

            ImGui::Begin("Set initial cells");

            if (ImGui::Button("Apply", ImVec2(40, 40))) {
                m_isInitialCellStateSet = true;
                m_isInitialized = true;                
            }
            
            for (std::uint16_t y = 0; y < m_gameAreaLenY; ++y) {
                for (std::uint16_t x = 0; x < m_gameAreaLenX; ++x) {
                    
                    std::uint32_t id = static_cast<std::uint32_t>(y) << 16 | x;
                    ImGui::PushID(id);
                    
                    if ((x % m_gameAreaLenX) != 0) {
                        ImGui::SameLine();
                    }

                    if (m_initialCellState[y][x] == 1) {
                        ImGui::PushStyleColor(
                            ImGuiCol_Button,
                            (ImVec4)ImColor::HSV(0.6f, 0.6f, 0.6f));
                        ImGui::PushStyleColor(
                            ImGuiCol_ButtonHovered,
                            (ImVec4)ImColor::HSV(4.0f, 0.7f, 0.7f));

                        if (ImGui::Button("1")) {
                            m_initialCellState[y][x] = 0;
                        }

                    }
                    else {
                        ImGui::PushStyleColor(
                            ImGuiCol_Button,
                            (ImVec4)ImColor::HSV(0.2f, 0.2f, 0.2f));
                        ImGui::PushStyleColor(
                            ImGuiCol_ButtonHovered,
                            (ImVec4)ImColor::HSV(4.0f, 0.7f, 0.7f));
                        
                        if (ImGui::Button("0")) {
                            m_initialCellState[y][x] = 1;
                        }
                    }
                    ImGui::PopStyleColor(2);
                    ImGui::PopID();
                }
            }
            ImGui::End();
        }
    }
    else {
        // Show keybindings and info / help
        // or maybe don't show anything ?
        // lets see.
    }
}

void GameOfLifeSetup::PImpl::startSceneUI()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void GameOfLifeSetup::PImpl::endSceneUI()
{
    ImGuiIO& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(m_manager->getWindowWidth(), m_manager->getWindowHeight());

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags /*& ImGuiConfigFlags_ViewportsEnable*/)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        glfwMakeContextCurrent(backup_current_context);
    }    
}


GameOfLifeSetup::GameOfLifeSetup(LayerManager* manager)
    : m_pImpl(std::make_unique<PImpl>(*this, manager))
{
    
}

GameOfLifeSetup::~GameOfLifeSetup()
{

}

void GameOfLifeSetup::init()
{
    m_pImpl->init();
}

void GameOfLifeSetup::onUpdate(float deltaTime)
{
    m_pImpl->onUpdate(deltaTime);
}

void GameOfLifeSetup::onRender()
{
    m_pImpl->onRender();
}

void GameOfLifeSetup::startSceneUI()
{
    m_pImpl->startSceneUI();
}

void GameOfLifeSetup::endSceneUI()
{
    m_pImpl->endSceneUI();
}
