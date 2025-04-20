# 添加版本等信息
set(BRANCH_NAME "Git_Unkown_branch")
set(COMMIT_HASH "Git_Unkown_commit")
set(COMMIT_TIME "Git_Unkown_time")
set(BUILD_TIME "")
set(BUILD_ENV
    "${CMAKE_SYSTEM_NAME} ${CMAKE_SYSTEM_VERSION} ${CMAKE_SYSTEM_PROCESSOR}")
set(COMPILER_NAME "${CMAKE_CXX_COMPILER_ID}")
set(BUILD_CXX_COMPILER "${COMPILER_NAME} ${CMAKE_CXX_COMPILER_VERSION}")
set(BUILD_C_COMPILER "${COMPILER_NAME} ${CMAKE_C_COMPILER_VERSION}")

# Git 操作
find_package(Git QUIET)
if(GIT_FOUND)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} symbolic-ref --short -q HEAD
    OUTPUT_VARIABLE BRANCH_NAME
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  message(STATUS "BRANCH_NAME: ${BRANCH_NAME}")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} rev-parse --short=7 HEAD
    OUTPUT_VARIABLE COMMIT_HASH
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  message(STATUS "COMMIT_HASH: ${COMMIT_HASH}")
  execute_process(
    COMMAND ${GIT_EXECUTABLE} log --format=format:%aI -1
    OUTPUT_VARIABLE COMMIT_TIME
    OUTPUT_STRIP_TRAILING_WHITESPACE ERROR_QUIET
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  message(STATUS "COMMIT_TIME: ${COMMIT_TIME}")
endif()

# 编译时间查询
string(TIMESTAMP BUILD_TIME "%Y-%m-%dT%H:%M:%S")
message(STATUS "BUILD_TIME: ${BUILD_TIME}")

# 读取库版本
file(STRINGS ${PROJECT_VERSION_PATH} RELEASE_VERSION)
message(STATUS "RELEASE_VERSION: ${RELEASE_VERSION}")

# 写入文件
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/include/wpml_codec/version.h.ini
               ${CMAKE_CURRENT_SOURCE_DIR}/include/wpml_codec/version.h @ONLY)
message(STATUS "Generate version.h")

file(
  GLOB_RECURSE
  CODE_FILES
  CONFIGURE_DEPENDS
  src/*.cpp
  src/*.c
  src/*.cc
  include/*.h
  include/*.hpp
  include/*/*.h
  include/*/*.hpp)

project(wpml_codec)

# 编译为动态库
add_library(${PROJECT_NAME} SHARED ${CODE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC ${DEP_LIBS} ${SYS_LIBS})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "ProjectTargets")

set(PROJ_LIB_NAME
    ${PROJECT_NAME}
    CACHE STRING "Library name")

# 安装
install(
  TARGETS ${PROJECT_NAME}
  LIBRARY DESTINATION ${SHARED_BIN_DIR}
  ARCHIVE DESTINATION lib
  RUNTIME DESTINATION bin)
install(DIRECTORY include/ DESTINATION include)
