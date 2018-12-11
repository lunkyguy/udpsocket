# 查找当前目录下的所有源文件
# 并将名称保存到 APP_SRCS 变量
#aux_source_directory(. APP_SRCS)
# file(GLOB_RECURSE APP_SRCS "*.h" "*.hpp" "*.cpp")
file(GLOB_RECURSE LIB_SRCS "dealMessage.h" "dealMessage.cpp" "main.cpp")
# 指定生成目标
add_library(dealMessage ${LIB_SRCS})
#添加链接库
target_link_libraries(dealMessage Qt5::Widgets)
