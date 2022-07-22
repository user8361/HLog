# 1、指定cmake最低版本
cmake_minimum_required(VERSION 3.2)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c++11 -Wall -Wno-deprecated  -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")
set(CMAKE_C_FLAGS "$ENV{CXXFLAGS} -rdynamic -O3 -fPIC -ggdb -std=c11 -Wall -Wno-deprecated -Wno-unused-function -Wno-builtin-macro-redefined -Wno-deprecated-declarations")

# 2、项目名称
project(HLOG)
# 3、头文件目录
include_directories(include include/log include/utils)

# 4、源文件目录
aux_source_directory(src SRC_LIST)
aux_source_directory(src/log SRC_LIST)
aux_source_directory(src/utils SRC_LIST)
aux_source_directory(test SRC_LIST)

# 5、设置环境变量，编译用到的源文件全部要放到这里
set(SRC ${SRC_LIST})

# 6、添加要编译的可执行文件
add_executable(${PROJECT_NAME} ${SRC})

# 7、添加可执行文件所需要的库
target_link_libraries(${PROJECT_NAME} yaml-cpp pthread )