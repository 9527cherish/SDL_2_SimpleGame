#pragma once

#include <future>
#include <memory>
#include <vector>
#include "persona.hpp"

// AssetRepository 负责加载和缓存客户端人物模板资源
class AssetRepository
{
public:
    // 全局只维护一份人物模板资源；运行时实例请通过 createPersona 克隆。
    static AssetRepository& getInstance();

    // 启动人物模板资源的异步加载。
    void init();
    // 返回当前所有已加载的人物模板。
    void getPersonas(std::vector<std::shared_ptr<Persona>>& personas);
    // 按索引克隆一个可运行的人物实例。
    std::shared_ptr<Persona> createPersona(int index);
    // 返回当前模板总数。
    int personaCount();

private:
    AssetRepository() = default;
    AssetRepository(const AssetRepository&) = delete;
    AssetRepository& operator=(const AssetRepository&) = delete;
    // 保证人物模板在访问前已经完成加载。
    void ensureLoaded();

private:
    // 已加载的人物模板列表
    std::vector<std::shared_ptr<Persona>> m_personas;
    // 资源是否已经加载完成
    bool m_loaded = false;
    // 异步加载任务句柄
    std::future<std::vector<std::shared_ptr<Persona>>> m_loadTask;
};
