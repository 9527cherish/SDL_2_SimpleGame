#include "interfaceManager.hpp"

InterfaceManager &InterfaceManager::getInstance()
{
    static InterfaceManager instance;
    return instance;
}