#include "GameOfLife.h"
#include "../LayerManager.h"
#include "../TemporaryRenderer.h"
#include "../Camera.h"
#include "../Shader.h"
#include "../Random.h"
#include "../CameraController.h"

#include <GLFW/glfw3.h>

#include <iostream>

struct GameOfLife::PImpl {
    GameOfLife& m_parent;
    LayerManager* m_manager;
    float m_initialZoomLevel;
    std::unique_ptr<CameraController> m_camera;

    std::vector<std::vector<std::uint32_t>> m_cellState;
    std::vector<std::vector<std::uint32_t>> m_cellStateSwp;

    bool m_isInitialized;
    GameMode m_gameMode;
    bool m_nextStep;
    
    std::uint32_t m_lenY;
    std::uint32_t m_lenX;

    float m_lastCursorXPos = 0.0f;
    float m_lastCursorYPos = 0.0f;
    float m_lastCursorYPosFixed = 0.0f;
    float m_lastCursorXPosFixed = 0.0f;
    float m_mouseClicked = false;
    
    PImpl(GameOfLife& parent, LayerManager* manager, float initialZoomLevel,
          const std::vector<std::vector<std::uint32_t>>& initState);
    ~PImpl();

    void init();
    void onUpdate(float deltaTime);
    void onRender();
    void onEvent(Event& e);
    
    void startSceneUI();
    void endSceneUI();

    void initGameStateRandom(std::size_t lenX, std::size_t lenY);
    void initGameStateFromSetupUI();
    
    void checkCells();
    void handleNextStep();
};

GameOfLife::PImpl::PImpl(
    GameOfLife& parent, LayerManager* manager, float initialZoomLevel,
    const std::vector<std::vector<std::uint32_t>>& initState)
    : m_parent(parent),
      m_manager(manager),
      m_camera(std::make_unique<CameraController>((1280.0f / 720.0f),
                                                  initialZoomLevel))
{
    m_isInitialized = false;
    m_camera->getCamera().setPosition({ 0.0f, 0.0f, 0.0f });
    if (initState.size() > 1) {
        m_cellState = initState;
        initGameStateFromSetupUI();
    }
}

GameOfLife::PImpl::~PImpl()
{

}

void GameOfLife::PImpl::init()
{
    m_gameMode = GameMode::Manual;
    m_nextStep = false;
}

void GameOfLife::PImpl::onUpdate(float deltaTime)
{
    m_camera->onUpdate(deltaTime);
    
    TemporaryRenderer::setClearColor();
    TemporaryRenderer::clear();

    TemporaryRenderer::start(m_camera->getCamera());

    // To have everything drawn around (0, 0, 0)
    std::int32_t positionFixX = (-1) * (m_lenX / 2.0f);
    std::int32_t positionFixY = (-1) * (m_lenY / 2.0f);
    std::int32_t posX;
    std::int32_t posY;
    
    for (std::uint32_t y = 0; y < m_lenY; y++) {

        posY = positionFixY + y;
        
        for (std::uint32_t x = 0; x < m_lenX; x++) {

            posX = positionFixX + x;
            
            if (m_mouseClicked) {
                m_mouseClicked = false;
            }

            if (m_cellState.at(y).at(x)) {

                TemporaryRenderer::drawQuad({ posY, posX, 0.0f },
                                            { 0.8f, 0.8f },
                                            { 0.67f, 0.17f, 0.27f, 1.0f});
            }
            
            else {
                
                TemporaryRenderer::drawQuad({ posY, posX, 0.0f },
                                            { 0.8f, 0.8f },
                                            { 0.2f, 0.2f, 0.2f, 1.0f});
            }
        }
        
    }

    if (m_gameMode == GameMode::Auto) {

        checkCells();

    }

    if (m_gameMode == GameMode::Manual && m_nextStep) {

        handleNextStep();
        
    }
    
    TemporaryRenderer::end();
}

void GameOfLife::PImpl::onRender()
{
}

void GameOfLife::PImpl::onEvent(Event& e)
{
    if (e.type() == EventType::KeyPressed || e.type() == EventType::KeyReleased) {
        if (e.keyValue() == GLFW_KEY_LEFT) {
            m_camera->onEvent(e);
        }
        if (e.keyValue() == GLFW_KEY_DOWN) {
            m_camera->onEvent(e);
        }
        if (e.keyValue() == GLFW_KEY_RIGHT) {
            m_camera->onEvent(e);
        }
        if (e.keyValue() == GLFW_KEY_UP) {
            m_camera->onEvent(e);
        }
    }
    
    if (e.type() == EventType::KeyPressed) {
        
        if (e.keyValue() == GLFW_KEY_X) {

            m_nextStep = true;
        }
        if (e.keyValue() == GLFW_KEY_R) {
            if (m_gameMode == GameMode::Auto) { return; }

            initGameStateRandom(m_lenX, m_lenY);
            m_gameMode = GameMode::Manual;
            m_nextStep = false;

        }
        if (e.keyValue() == GLFW_KEY_SPACE) {
            m_nextStep = false;
            if (m_gameMode == GameMode::Auto) {
                m_gameMode = GameMode::Manual;
            } else {
                m_gameMode = GameMode::Auto;
            }            
        }

    }

    if (e.type() == EventType::MouseMoved) {
        const auto& fetchedValues = e.mouseValues();
        m_lastCursorXPos = fetchedValues.first;
        m_lastCursorYPos = fetchedValues.second;
        // If mouse goes outside of window; can user click mousebutton?
        // maybe come back to this if, again, ever needed...
    }

    if (e.type() == EventType::MouseScrolled) {
        m_camera->onEvent(e);
    }

    if (e.type() == EventType::MouseClicked) {
        // This does not work and will require modifications to rendering.
        if (e.keyValue() == GLFW_MOUSE_BUTTON_LEFT) {
            m_lastCursorXPos = e.mouseValues().first;
            m_lastCursorYPos = e.mouseValues().second;
            m_mouseClicked = true;
        }
    }
}

void GameOfLife::PImpl::initGameStateRandom(std::size_t lenX, std::size_t lenY)
{
    m_cellState = std::vector(lenY, std::vector<std::uint32_t>(lenX));
    m_cellStateSwp = std::vector(lenY, std::vector<std::uint32_t>(lenX));
    
    m_lenX = lenX;
    m_lenY = lenY;

    // Put each cell active with, say 20% chance
    float startsAlive = 0.0f;
    for (std::size_t y = 0; y < lenY; ++y) {
        
        for (std::size_t x = 0; x < lenX; ++x) {

            startsAlive = Random::Float();
            if (startsAlive < 0.2f) {

                m_cellState.at(y).at(x) = 1;

            }

        }
        
    }   
}

void GameOfLife::PImpl::initGameStateFromSetupUI()
{
        
    m_lenX = m_cellState[0].size();
    m_lenY = m_cellState.size();
    m_cellStateSwp = std::vector(m_lenY, std::vector<std::uint32_t>(m_lenX));
}

void GameOfLife::PImpl::checkCells()
{
    std::uint32_t currCell;
    std::uint32_t numNeighbors;

    bool hasLeft = true;
    bool hasRight = true;
    bool hasTop = true;
    bool hasBottom = true;    
    
    for (std::size_t y = 0; y < m_lenY; ++y) {
        
        for (std::size_t x = 0; x < m_lenX; ++x) {

            numNeighbors = 0;
            hasLeft = true;
            hasRight = true;
            hasTop = true;
            hasBottom = true;    

            currCell = m_cellState.at(y).at(x);
            m_cellStateSwp[y][x] = currCell;
            
            if (y == 0) {
                hasTop = false;
            }

            if (x == 0) {
                hasLeft = false;
            }

            if (y == (m_lenY - 1)) {
                hasBottom = false;
            }

            if (x == (m_lenX - 1)) {
                hasRight = false;
            }

            if (hasTop) {
                if (m_cellState.at(y - 1).at(x)) { numNeighbors++; }
            }
            if (hasLeft) {
                if (m_cellState.at(y).at(x - 1)) { numNeighbors++; }
            }
            if (hasBottom) {
                if (m_cellState.at(y + 1).at(x)) { numNeighbors++; }
            }
            if (hasRight) {
                if (m_cellState.at(y).at(x + 1)) { numNeighbors++; }
            }
            
            if (hasTop && hasLeft) {
                if (m_cellState.at(y - 1).at(x - 1)) { numNeighbors++; }
            }
            if (hasBottom && hasLeft) {
                if (m_cellState.at(y + 1).at(x - 1)) { numNeighbors++; }
            }
            if (hasBottom && hasRight) {
                if (m_cellState.at(y + 1).at(x + 1)) { numNeighbors++; }
            }
            if (hasTop && hasRight) {
                if (m_cellState.at(y - 1).at(x + 1)) { numNeighbors++; }
            }

            if (currCell) {
                
                if (numNeighbors < 2) {
                    m_cellStateSwp[y][x] = 0;
                }
                if (numNeighbors > 3) {
                    m_cellStateSwp[y][x] = 0;
                }
                
            } else {

                if (numNeighbors == 3) {
                    m_cellStateSwp[y][x] = 1;
                }
                
            }
                        
        }

    }

    for (std::uint32_t swpRow = 0; swpRow < m_lenY; swpRow++) {
        std::vector<std::uint32_t>& rowOld = m_cellState[swpRow];
        std::vector<std::uint32_t>& rowNew = m_cellStateSwp[swpRow];
        rowOld.swap(rowNew);
    }
    
}

void GameOfLife::PImpl::handleNextStep()
{
    checkCells();
    m_nextStep = false;
}

GameOfLife::GameOfLife(LayerManager* manager,
                       const std::vector<std::vector<std::uint32_t>>& initState)
    : m_pImpl(std::make_unique<PImpl>(*this, manager, 40.0f, initState))
{

}

GameOfLife::~GameOfLife()
{

}

void GameOfLife::init()
{
    m_pImpl->init();
}

void GameOfLife::onUpdate(float deltaTime)
{
    m_pImpl->onUpdate(deltaTime);
}

void GameOfLife::onRender()
{
    m_pImpl->onRender();
}

void GameOfLife::onEvent(Event& e)
{
    m_pImpl->onEvent(e);
}

void GameOfLife::initializedFromSetup()
{
    m_pImpl->m_isInitialized = true;
}
