# 收集源文件
file(GLOB_RECURSE SOURCES 
    "src/*.cpp"
    "src/*.c"
)

# 创建第三方库
add_library(example_lib ${SOURCES})

# 指定头文件搜索路径
target_include_directories(example_lib
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)