# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.5)
# 项目信息
project (udp)
#添加QT库
find_package(Qt5Widgets)
# 在相应的构建目录中查找包含目录
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Cmake在需要时自动运行MOC
set(CMAKE_AUTOMOC ON)
# 从Qt设计器UI文件列表创建代码
set(CMAKE_AUTOUIC ON)
# 添加子目录
add_subdirectory(dealMessage)
# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_SRCS 变量
#aux_source_directory(. DIR_SRCS)
set(APP_SRCS
    mainwindow.ui
	mainwindow.h
    mainwindow.cpp
    main.cpp
)
# 指定生成目标
add_executable(udp ${APP_SRCS})
# 添加链接库
target_link_libraries(udp dealMessage)
#屏蔽控制台
Set_Target_Properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS_RELEASE "/SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")