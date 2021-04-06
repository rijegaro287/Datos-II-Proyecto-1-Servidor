cmake_minimum_required(VERSION 3.17)
project(Datos_II_Proyecto_1_Servidor)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)
find_package(jsoncpp CONFIG REQUIRED)

add_executable(Datos_II_Proyecto_1_Servidor Memory/MemoryPool.cpp Memory/MemoryPool.h Memory/MemoryBlock.h Memory/MemoryNode.h Scope/Scope.cpp VariableManager.cpp Communication/Socket.h Communication/Socket.cpp main.cpp)

target_link_libraries(Datos_II_Proyecto_1_Servidor PRIVATE jsoncpp_object jsoncpp_static)
