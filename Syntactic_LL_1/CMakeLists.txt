# CMakeList.txt: Syntactic_LL_1 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

# 将源代码添加到此项目的可执行文件。
add_executable (Syntactic_LL_1 
	"Syntactic_LL_1.cpp" 
	"Syntactic_LL_1.h" 
	"Generator.cpp"
	"Generator.h"
 "main.cpp")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Syntactic_LL_1 PROPERTY CXX_STANDARD 20)
endif()

# TODO: 如有需要，请添加测试并安装目标。
