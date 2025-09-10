#pragma once 


#include "characterStruct.hpp"
#include "persona.hpp"
#include <future>


// 管理所有数据

class DataManager
{

public:
    static DataManager& getInstance(); 
    DataManager();
    ~DataManager();

    // 初始化数据
    void initData();
    void getData(std::vector<Persona>& personas);

    
    // bool getPersonas(std::vector<Persona>& persons);
    // 单例将拷贝构造和赋值构造删除
    DataManager(const DataManager&) = delete;
    DataManager(const DataManager&&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    DataManager& operator=(const DataManager&&) = delete;

private:
    void getDataFromFuture();
    std::vector<Persona> m_personas;
    bool  m_bLoadData;
    std::future<std::vector<Persona>> m_loadData;

};