#include "SandboxLayer.h"
#include "../LayerManager.h"
#include "../TemporaryRenderer.h"
#include "../Camera.h"
#include "../Shader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

struct SandboxLayer::PImpl {
    SandboxLayer& m_parent;
    LayerManager* m_manager;
    
    PImpl(SandboxLayer& parent, LayerManager* manager);
    ~PImpl();

    void init();
    void onUpdate(float deltaTime);
    void onRender();

    void startSceneUI();
    void endSceneUI();
};

SandboxLayer::PImpl::PImpl(SandboxLayer& parent, LayerManager* manager)
    : m_parent(parent),
      m_manager(manager)
{

}

SandboxLayer::PImpl::~PImpl()
{
}

void SandboxLayer::PImpl::init()
{
    
}

void SandboxLayer::PImpl::onUpdate(float deltaTime)
{

}

void SandboxLayer::PImpl::onRender()
{
}


SandboxLayer::SandboxLayer(LayerManager* manager)
    : m_pImpl(std::make_unique<PImpl>(*this, manager))
{
    
}

SandboxLayer::~SandboxLayer()
{

}

void SandboxLayer::init()
{
    m_pImpl->init();
}

void SandboxLayer::onUpdate(float deltaTime)
{
    m_pImpl->onUpdate(deltaTime);
}

void SandboxLayer::onRender()
{
    m_pImpl->onRender();
}

