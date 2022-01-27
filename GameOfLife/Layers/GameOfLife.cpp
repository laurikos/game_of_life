#include "GameOfLife.h"
#include "../LayerManager.h"
#include "../TemporaryRenderer.h"
#include "../Camera.h"
#include "../Shader.h"
#include "../Random.h"

#include <GLFW/glfw3.h>

#include <iostream>

struct GameOfLife::PImpl {
    GameOfLife& m_parent;
    LayerManager* m_manager;
    std::unique_ptr<Camera> m_camera;

    std::vector<std::vector<std::uint32_t>> m_cellState;

    std::uint32_t m_lenY;
    std::uint32_t m_lenX;
    
    PImpl(GameOfLife& parent, LayerManager* manager);
    ~PImpl();

    void init();
    void onUpdate(float deltaTime);
    void onRender();
    void onEvent(Event& e);
    
    void startSceneUI();
    void endSceneUI();

    void initGameState(std::size_t lenX, std::size_t lenY);
    void checkCells();
};

GameOfLife::PImpl::PImpl(GameOfLife& parent, LayerManager* manager)
    : m_parent(parent),
      m_manager(manager),
      m_camera(std::make_unique<Camera>(
                   -1 * (1200.0f / 720.0f) * 3.0f,
                   (1200.0f / 720.0f) * 3.0f,
                   -1 * 3.0f,
                   3.0f))
{
    m_camera->setPosition({ 0.0f, 0.0f, 0.0f });
}

GameOfLife::PImpl::~PImpl()
{
}

void GameOfLife::PImpl::init()
{
    initGameState(100, 100);
}

void GameOfLife::PImpl::onUpdate(float deltaTime)
{
    TemporaryRenderer::setClearColor();
    TemporaryRenderer::clear();

    TemporaryRenderer::start(m_camera.get());


    std::int32_t positionFixX = (-1) * (m_lenX / 2.0f);
    std::int32_t positionFixY = (-1) * (m_lenY / 2.0f);
    std::int32_t posX;
    std::int32_t posY;
    
    for (std::uint32_t i = 0; i < m_lenY; ++i) {

        posY = positionFixY + i;
        
        for (std::uint32_t j = 0; j < m_lenX; ++j) {

            posX = positionFixX + j;
            
            if (m_cellState.at(i).at(j)) {
                TemporaryRenderer::drawQuad({ (posY/10.0f), (posX/10.0f), 0.0f },
                                            { 0.08f, 0.08f },
                                            { 0.67f, 0.17f, 0.27f, 1.0f});
            } else {
                TemporaryRenderer::drawQuad({ (posY/10.0f), (posX/10.0f), 0.0f },
                                            { 0.08f, 0.08f },
                                            { 0.2f, 0.2f, 0.2f, 1.0f});

            }
        }
        
    }

    checkCells();
    
    TemporaryRenderer::end();
}

void GameOfLife::PImpl::onRender()
{
}

void GameOfLife::PImpl::onEvent(Event& e)
{
    if (e.type() == EventType::KeyPressed) {
        if (e.keyValue() == GLFW_KEY_RIGHT) {
            printf("%s\n", "LEFT KEY PRESSED!");
        }
    }
}

void GameOfLife::PImpl::initGameState(std::size_t lenX, std::size_t lenY)
{
    m_cellState = std::vector(lenY, std::vector<std::uint32_t>(lenX));

    m_lenX = lenX;
    m_lenY = lenY;
    // Start with cell one:
    // for automation have, say 30% alive ?
    float startsAlive = 0.0f;
    for (std::size_t y = 0; y < lenY; ++y) {
        
        for (std::size_t x = 0; x < lenX; ++x) {

            startsAlive = Random::Float();
            if (startsAlive < 0.3f) {

                m_cellState.at(y).at(x) = 1;

            }

        }
        
    }   
}

void GameOfLife::PImpl::checkCells()
{
    std::uint32_t currCell;
    std::uint32_t numNeighbors;

    bool hasLeft = true;
    bool hasRight = true;
    bool hasTop = true;
    bool hasBottom = true;    

    // std::cout << "\n[ ";    
    // for (std::size_t y = 0; y < m_lenY; ++y) {

    //     std::cout << "\n{";
        
    //     for (std::size_t x = 0; x < m_lenX; ++x) {

    //         std:: cout << " " << m_cellState.at(y).at(x) << " ";
    //     }
        
    //     std::cout << "}\n";

    // }
    
    // std::cout << "] ";
    // std::cout << "\n" << std::endl;
    
    for (std::size_t y = 0; y < m_lenY; ++y) {
        
        for (std::size_t x = 0; x < m_lenX; ++x) {

            numNeighbors = 0;
            hasLeft = true;
            hasRight = true;
            hasTop = true;
            hasBottom = true;    

            currCell = m_cellState.at(y).at(x);

            if (y == 0) {
                hasTop = false;
            }

            if (x == 0) {
                hasLeft = false;
            }

            if (y == (m_lenY - 1)) {
                hasBottom = false;
            }

            if (x == (m_lenX -1)) {
                hasRight = false;
            }

            if (hasTop) {
                
                if (!hasLeft) {
                    for (std::uint32_t topX = x; topX < x + 1; topX++) {
                        if (m_cellState.at(y - 1).at(topX)) { numNeighbors++; }
                    }
                }
                else if (!hasRight) {
                    for (std::uint32_t topX = x - 1; topX < x; topX++) {
                        if (m_cellState.at(y - 1).at(topX)) { numNeighbors++; }
                    }
                }
                else if (hasRight && hasLeft) {
                    for (std::uint32_t topX = x - 1; topX < x + 1; topX++) {
                        if (m_cellState.at(y - 1).at(topX)) { numNeighbors++; }
                    }
                }
                
            }

            else if (hasBottom) {
                
                if (!hasLeft) {
                    for (std::uint32_t botX = x; botX < x + 1; botX++) {
                        if (m_cellState.at(y + 1).at(botX)) { numNeighbors++; }
                    }
                }
                else if (!hasRight) {
                    for (std::uint32_t botX = x - 1; botX < x; botX++) {
                        if (m_cellState.at(y + 1).at(botX)) { numNeighbors++; }
                    }
                }
                else if (hasRight && hasLeft) {
                    for (std::uint32_t botX = x - 1; botX < x + 1; botX++) {
                        if (m_cellState.at(y + 1).at(botX)) { numNeighbors++; }
                    }
                }

            }

            if (hasRight) {

                if (!hasTop) {
                    for (std::uint32_t rightY = y; rightY < y + 1; rightY++) {
                        if (m_cellState.at(rightY).at(x+1)) { numNeighbors++; }
                    }
                }
                else if (!hasBottom) {
                    for (std::uint32_t rightY = y-1; rightY < y; rightY++) {
                        if (m_cellState.at(rightY).at(x+1)) { numNeighbors++; }
                    }
                }
                else if (hasTop && hasBottom) {
                    for (std::uint32_t rightY = y-1; rightY < y + 1; rightY++) {
                        if (m_cellState.at(rightY).at(x+1)) { numNeighbors++; }
                    }
                }
            }

            else if (hasLeft) {
                if (!hasTop) {
                    for (std::uint32_t rightY = y; rightY < y + 1; rightY++) {
                        if (m_cellState.at(rightY).at(x-1)) { numNeighbors++; }
                    }
                }
                else if (!hasBottom) {
                    for (std::uint32_t rightY = y-1; rightY < y; rightY++) {
                        if (m_cellState.at(rightY).at(x-1)) { numNeighbors++; }
                    }
                }
                else if (hasTop && hasBottom) {
                    for (std::uint32_t rightY = y-1; rightY < y + 1; rightY++) {
                        if (m_cellState.at(rightY).at(x-1)) { numNeighbors++; }
                    }
                }

            }

            if (currCell) {
                
                if (numNeighbors < 2) {
                    m_cellState[y][x] = 0;
                }
                if (numNeighbors > 3) {
                    m_cellState[y][x] = 0;
                }
                
            } else {

                if (numNeighbors == 3) {
                    m_cellState[y][x] = 1;
                }
                
            }
                        
        }

    }
}

GameOfLife::GameOfLife(LayerManager* manager)
    : m_pImpl(std::make_unique<PImpl>(*this, manager))
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
