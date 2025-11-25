# 收集源文件
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.c"
)

# 创建通用库
add_library(rk3568_drive_lib ${SOURCES})

# 指定头文件搜索路径
target_include_directories(rk3568_drive_lib
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)