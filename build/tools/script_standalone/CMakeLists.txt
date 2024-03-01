cmake_minimum_required(VERSION 3.1)
project(gsc)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

add_definitions(-DGLM_FORCE_CTOR_INIT)
get_filename_component(PARENT_DIR ../../../ ABSOLUTE)
include_directories(${PARENT_DIR}/src)
include_directories(${PARENT_DIR}/third_party/include)
include_directories(${PARENT_DIR}/third_party)

add_executable(
gsc
#../../../third_party/include/miniz/miniz.c
../../../src/core/time.cpp
#../../../src/common/logger.cpp
../../../src/common/filesystem.cpp
../../../src/core/filesystem/api.cpp
../../../src/script/ast/ast_generator.cpp
../../../src/script/ast/gsc_writer.cpp
../../../src/script/ast/recursive_visitor.cpp
../../../src/parse/preprocessor.cpp
../../../src/script/ast/visitor.cpp
../../../src/script/compiler/compiler.cpp
../../../src/script/stockfunctions.cpp
../../../src/script/vm/instructions/instructions.cpp
../../../src/script/vm/virtual_machine.cpp
../../../src/tools/script_standalone/script_standalone.cpp
)

add_custom_target(
  run
  COMMAND ${CMAKE_PROJECT_NAME}
  DEPENDS ${CMAKE_PROJECT_NAME}
  WORKING_DIRECTORY ${CMAKE_PROJECT_DIR}
)
