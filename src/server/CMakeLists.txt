add_executable(server)

target_sources(server PRIVATE
    main.cpp
    gameServer.cpp
    gameService.cpp
    serverConnectionRegistry.cpp
    serverMonsterManager.cpp
    serverMessageRouter.cpp
    serverPlayerStore.cpp
    serverComonFunc.cpp
    serverTreeManager.cpp
)

link_directories(${CMAKE_SOURCE_DIR}/lib)

target_link_libraries(server PRIVATE
    muduo_net muduo_base mysqlclient pthread spdlog::spdlog
)
