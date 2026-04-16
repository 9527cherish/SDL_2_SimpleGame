cmake_minimum_required(VERSION 3.10)
project(rougeClient VERSION 1.0.0 LANGUAGES CXX)

message("CMAKE_CURRENT_SOURCE_DIR is ${CMAKE_CURRENT_SOURCE_DIR}")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(BIN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/bin)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_Mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)

include_directories(${UUID_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS})

link_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(rougeClient)

target_sources(rougeClient PRIVATE
    main.cpp
    clientComonFunc.cpp
    Network/clientTransport.cpp
    Network/netClient.cpp
    BasicControls/button.cpp
    BasicControls/camera.cpp
    BasicControls/cursor.cpp
    Character/characterAction.cpp
    Character/characterDirection.cpp
    Character/partBase.cpp
    Character/persona.cpp
    Character/remotepersona.cpp
    DataManager/assetRepository.cpp
    DataManager/interfaceManager.cpp
    DataManager/loadXml.cpp
    DataManager/playerSession.cpp
    DataManager/remotePlayerStore.cpp
    DataManager/treeManager.cpp
    GameInterface/gameInterface.cpp
    GameInterface/gameScene.cpp
    GameInterface/mainScene.cpp
    GameInterface/settingScene.cpp
    GameInterface/worldScenePresenter.cpp
)

target_link_libraries(rougeClient PRIVATE
    ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY} ${SDL2_TTF_LIBRARIES}
    pugixml spdlog::spdlog ${UUID_LIBRARIES}
)
