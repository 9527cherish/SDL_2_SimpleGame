#pragma once 


#include "characterStruct.hpp"
#include "persona.hpp"
#include <future>
#include <memory>

// 管理所有数据

class DataManager
{

public:
    static DataManager& getInstance(); 

    DataManager();
    ~DataManager();

    // 初始化数据
    void initData();
    void getData(std::vector<std::shared_ptr<Persona>>& personas);

    
    void setCurrentPerson(std::shared_ptr<Persona> persona);
    void setCurrentPerson(int number);
    std::shared_ptr<Persona> currentPersona();

    // bool getPersonas(std::vector<Persona>& persons);
    // 单例将拷贝构造和赋值构造删除
    DataManager(const DataManager&) = delete;

    DataManager(const DataManager&&) = delete;
    DataManager& operator=(const DataManager&) = delete;
    DataManager& operator=(const DataManager&&) = delete;

private:
    void getDataFromFuture();
    std::vector<std::shared_ptr<Persona>> m_personas;
    bool  m_bLoadData;
    std::future<std::vector<std::shared_ptr<Persona>>> m_loadData;

    // 当前选中的人物形象
    std::shared_ptr<Persona> m_pCurrentPerson;

};