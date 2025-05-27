cmake_minimum_required(VERSION 3.15 FATAL_ERROR)

# C/C++配置
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(LINK_WHAT_YOU_USE ON)

# 开启目录层次结构
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 设置CMake策略
if(POLICY CMP0078)
  cmake_policy(SET CMP0078 NEW)
endif()
if(POLICY CMP0086)
  cmake_policy(SET CMP0086 NEW)
endif()

# 项目
project(wpml_codec_project LANGUAGES C CXX)

# 可选编译内容
option(BUILD_SAMPLE "Build sample" ON)

# 防止在源码目录下进行内部构建
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
  message(
    FATAL_ERROR "Please create a build directory and run CMake from there")
endif()

# 判断是否为子项目
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  message(STATUS "This is a top project")
  set(PROJECT_FOLDER_DIR "ProjectTargets")
else()
  message(STATUS "This is a sub project")
  set(BUILD_SAMPLE OFF)
  set(PROJECT_FOLDER_DIR "ExtraTargets")
endif()

# 包含工具
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include(Functions)

# 路径设置
set(PROJECT_ROOT_DIR "${PROJECT_SOURCE_DIR}")
message(STATUS "PROJECT_ROOT_DIR: ${PROJECT_ROOT_DIR}")
set(PROJECT_DEPENDS_DIR "${PROJECT_ROOT_DIR}/3rd")
message(STATUS "PROJECT_DEPENDS_DIR: ${PROJECT_DEPENDS_DIR}")
set(PROJECT_VERSION_PATH "${PROJECT_ROOT_DIR}/VERSION")
message(STATUS "PROJECT_VERSION_PATH: ${PROJECT_VERSION_PATH}")

# 读取缓存
find_program(CCACHE_FOUND ccache)
if(CCACHE_FOUND)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE ccache)
  set_property(GLOBAL PROPERTY RULE_LAUNCH_LINK ccache)
endif()

# 编译器选项
if(MSVC)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/utf-8>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/W4>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/wd4828$<SEMICOLON>/wd4251>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/EHsc>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/Zc:__cplusplus>)
  add_compile_options($<$<CXX_COMPILER_ID:MSVC>:/Zc:preprocessor>)
else()
  add_compile_options($<$<CXX_COMPILER_ID:GNU,Clang>:-fPIC>)
  add_compile_options($<$<CXX_COMPILER_ID:GNU,Clang>:-Wall$<SEMICOLON>-Wextra>)
  add_compile_options($<$<CXX_COMPILER_ID:GNU,Clang>:-Wno-unknown-pragmas>)
  add_link_options($<$<CXX_COMPILER_ID:GNU,Clang>:-flto>)
  if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_options(-g3)
  else()
    add_compile_options(-g0)
  endif()
endif()

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU"
   AND CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 10.0.0
   AND CMAKE_CXX_COMPILER_VERSION VERSION_LESS 11.0.0)
  add_compile_options(
    $<$<COMPILE_LANG_AND_ID:CXX,GNU>:-Wno-error$<SEMICOLON>-fcoroutines$<SEMICOLON>-D__cplusplus=202002L>
  )
  if(IS_DIRECTORY /usr/include/c++/10)
    include_directories($<$<COMPILE_LANG_AND_ID:CXX,GNU>:/usr/include/c++/10>)
  endif()
endif()

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

# 三方库依赖
set(tinyxml2_BUILD_TESTING
    OFF
    CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_DEPENDS_DIR}/tinyxml2")
set_property(TARGET tinyxml2 PROPERTY FOLDER "ExtraTargets")
update_cached_list(DEP_LIBS tinyxml2)

set(ZLIB_BUILD_TESTING
    OFF
    CACHE BOOL "" FORCE)
set(ZLIB_BUILD_MINIZIP
    ON
    CACHE BOOL "" FORCE)
set(MINIZIP_BUILD_TESTING
    OFF
    CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_DEPENDS_DIR}/zlib")
set_property(TARGET zlib PROPERTY FOLDER "ExtraTargets")
set_property(TARGET zlibstatic PROPERTY FOLDER "ExtraTargets")
set_property(TARGET libminizip PROPERTY FOLDER "ExtraTargets")
set_property(TARGET libminizipstatic PROPERTY FOLDER "ExtraTargets")
set_property(TARGET minizip PROPERTY FOLDER "ExtraTargets")
set_property(TARGET minizipstatic PROPERTY FOLDER "ExtraTargets")
set_property(TARGET miniunzip PROPERTY FOLDER "ExtraTargets")
set_property(TARGET miniunzipstatic PROPERTY FOLDER "ExtraTargets")
update_cached_list(DEP_LIBS zlibstatic libminizipstatic)

set(MAGIC_ENUM_OPT_BUILD_EXAMPLES
    OFF
    CACHE BOOL "" FORCE)
set(MAGIC_ENUM_OPT_BUILD_TESTS
    OFF
    CACHE BOOL "" FORCE)
add_subdirectory("${PROJECT_DEPENDS_DIR}/magic_enum")
update_cached_list(DEP_LIBS magic_enum)

if(WIN32)
  update_cached_list(SYS_LIBS WS2_32 Iphlpapi shlwapi)
elseif(NOT ANDROID OR IOS)
  update_cached_list(SYS_LIBS pthread)
else()
  update_cached_list(SYS_LIBS stdc++)
endif()

# 添加子目录
add_subdirectory(wpml_codec)
set(wpml_codec_HEADER_DIR "${PROJECT_ROOT_DIR}/wpml_codec/include")

# 是否编译生成示例
if(BUILD_SAMPLE)
  add_subdirectory(sample)
endif()
