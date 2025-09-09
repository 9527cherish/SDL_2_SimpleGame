
#include "clientComonFunc.hpp"
#include "characterStruct.hpp"
#include "dataManager.hpp"
#include "interfaceManager.hpp"
#include "gameInterface.hpp"

int main() {  // int argc, char* argv[]

    ClientComonFunc::getInstance();
    DataManager::getInstance().initData();
    InterfaceManager::getInstance().start();
    
    return 0;
}
