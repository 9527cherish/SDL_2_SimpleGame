
aux_source_directory(. SRC_LIST)

# 设置生成动态库
add_library(publicFunc SHARED ${SRC_LIST})

# 设置 .so 文件输出到 build/lib 目录下
set_target_properties(publicFunc PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/lib
)