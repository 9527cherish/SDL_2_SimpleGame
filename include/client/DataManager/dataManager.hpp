#pragma once 


#include "publicStruct.hpp"
#include "persona.hpp"

// 管理所有数据

class DataManager
{

public:
    static DataManager& getInstance(); 
    DataManager();
    ~DataManager();

    // 初始化数据
    void initData();

    // 单例将拷贝构造和赋值构造删除
    DataManager(const DataManager&) = delete;
    DataManager(const DataManager&&) = delete;

    DataManager& operator=(const DataManager&) = delete;
    DataManager& operator=(const DataManager&&) = delete;

private:

    std::vector<Persona> m_personas;

};