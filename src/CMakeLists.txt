# 收集源文件
file(GLOB_RECURSE SOURCES 
    "*.cpp"
    "*.c"
)

# 创建可执行文件
add_executable(mainapp ${SOURCES})

# 指定头文件搜索路径
target_include_directories(mainapp
    PRIVATE 
        ${CMAKE_SOURCE_DIR}/include
)

# 链接第三方库和通用库
target_link_libraries(mainapp PRIVATE 
                    example_lib 
                    rk3568_drive_lib)
