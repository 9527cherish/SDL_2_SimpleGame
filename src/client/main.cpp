
#include "clientComonFunc.hpp"
#include "characterStruct.hpp"
#include "dataManager.hpp"
#include "interfaceManager.hpp"

int main(int argc, char* argv[]) {

    ClientComonFunc::getInstance();
    // DataManager::getInstance().initData();
    InterfaceManager::getInstance().initWindow();



    InterfaceManager::getInstance().closeWindow();
    
    return 0;
}
