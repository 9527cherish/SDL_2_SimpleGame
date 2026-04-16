#include "assetRepository.hpp"
#include "loadXml.hpp"

AssetRepository& AssetRepository::getInstance()
{
    static AssetRepository instance;
    return instance;
}

void AssetRepository::init()
{
    if (m_loaded || m_loadTask.valid()) {
        return;
    }

    spdlog::info("开始异步加载人物资源模板");
    m_loadTask = std::async(std::launch::async, &LoadXml::parseAllPersonaXml);
}

void AssetRepository::getPersonas(std::vector<std::shared_ptr<Persona>>& personas)
{
    ensureLoaded();
    personas = m_personas;
}

std::shared_ptr<Persona> AssetRepository::createPersona(int index)
{
    ensureLoaded();
    if (index < 0 || index >= static_cast<int>(m_personas.size())) {
        spdlog::error("创建人物失败，索引越界: {}", index);
        return nullptr;
    }

    return std::make_shared<Persona>(*(m_personas[index]));
}

int AssetRepository::personaCount()
{
    ensureLoaded();
    return static_cast<int>(m_personas.size());
}

void AssetRepository::ensureLoaded()
{
    if (m_loaded) {
        return;
    }

    if (!m_loadTask.valid()) {
        init();
    }

    spdlog::info("等待人物资源模板加载完成");
    m_personas = m_loadTask.get();
    m_loaded = true;
    spdlog::info("人物资源模板加载完成，数量: {}", m_personas.size());
}
