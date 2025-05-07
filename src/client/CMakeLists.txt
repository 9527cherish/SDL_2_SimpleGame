cmake_minimum_required(VERSION 2.6)
project(client)

# Use our modified FindSDL2* modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${client_SOURCE_DIR}/cmake")
set(BIN_DIR ${client_SOURCE_DIR}/bin)



find_package(SDL2 REQUIRED)
find_package(SDL2_Image REQUIRED)
find_package(SDL2_Mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)
# find_package(PugiXML REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(UUID REQUIRED uuid)

# 包含目录
include_directories(${UUID_INCLUDE_DIRS})

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIRS})

aux_source_directory(. SRC_LIST)
aux_source_directory(./GameInterface GAME_LIST)
aux_source_directory(./HttpServer HTTP_LIST)

link_directories(${CMAKE_SOURCE_DIR}/lib)

# 编译动态库
# add_library(MuduoSimplify SHARED ${SRC_LIST})
add_executable(client ${SRC_LIST} ${GAME_LIST} ${HTTP_LIST})

target_link_libraries(client PRIVATE 
                ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2_MIXER_LIBRARY}  ${SDL2_TTF_LIBRARIES} 
                pugixml spdlog::spdlog ${UUID_LIBRARIES} publicFunc)

# 安装规则
# install(TARGETS client RUNTIME DESTINATION ${BIN_DIR})
