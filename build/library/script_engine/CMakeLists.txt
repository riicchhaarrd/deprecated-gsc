# cmake --build . --config Release
cmake_minimum_required(VERSION 3.1)
# https://stackoverflow.com/questions/6241922/how-to-properly-set-cmake-install-prefix-from-the-command-line
# cmake -DCMAKE_INSTALL_PREFIX=/usr ..
# SET(CMAKE_INSTALL_PREFIX "D:/install")
project(script_engine)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_definitions(-DGLM_FORCE_CTOR_INIT)
get_filename_component(PARENT_DIR ${CMAKE_CURRENT_LIST_DIR}/../../../ ABSOLUTE)
#include_directories(${PARENT_DIR}/src)
#include_directories(${PARENT_DIR}/third_party/include)
#include_directories(${PARENT_DIR}/third_party)

add_library(
script_engine
STATIC
${PARENT_DIR}/third_party/include/miniz/miniz.c
${PARENT_DIR}/src/core/time.cpp
#${PARENT_DIR}/src/common/logger.cpp
${PARENT_DIR}/src/common/filesystem.cpp
${PARENT_DIR}/src/core/filesystem/api.cpp
${PARENT_DIR}/src/script/ast/ast_generator.cpp
${PARENT_DIR}/src/script/ast/gsc_writer.cpp
${PARENT_DIR}/src/script/ast/recursive_visitor.cpp
${PARENT_DIR}/src/parse/preprocessor.cpp
${PARENT_DIR}/src/script/ast/visitor.cpp
${PARENT_DIR}/src/script/compiler/compiler.cpp
${PARENT_DIR}/src/script/stockfunctions.cpp
${PARENT_DIR}/src/script/vm/instructions/instructions.cpp
${PARENT_DIR}/src/script/vm/virtual_machine.cpp
#../../../src/tools/script_standalone/script_standalone.cpp
)
target_include_directories(script_engine
    PRIVATE
    ${PARENT_DIR}/src
    ${PARENT_DIR}/third_party/
    ${PARENT_DIR}/third_party/include
)

# install(TARGETS script_engine LIBRARY DESTINATION lib)
# msbuild <solution> /p:Configuration=Release
