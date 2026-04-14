#include "clientComonFunc.hpp"
#include "characterStruct.hpp"
#include "dataManager.hpp"
#include "interfaceManager.hpp"
#include "gameInterface.hpp"

int main() {  // int argc, char* argv[]
    ClientComonFunc::installCrashHandlers();

    ClientComonFunc::getInstance();
    DataManager::getInstance().initData();
    
    try {
        InterfaceManager::getInstance().start();
    } catch (const std::exception& ex) {
        ClientComonFunc::showCrashDialog("崩溃", ex.what());
        return 1;
    } catch (...) {
        ClientComonFunc::showCrashDialog("崩溃", "程序发生了未知异常。");
        return 1;
    }
    
    return 0;
}
