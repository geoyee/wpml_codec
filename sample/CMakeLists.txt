file(GLOB_RECURSE CODE_FILES CONFIGURE_DEPENDS *.cpp *.c *.cc)

project(wpml_codec_sample)

# 编译为可执行文件
add_executable(${PROJECT_NAME} ${CODE_FILES})
target_link_libraries(${PROJECT_NAME} PUBLIC ${PROJ_LIB_NAME})
set_property(TARGET ${PROJECT_NAME} PROPERTY FOLDER "ProjectTargets")

# 拷贝数据
set(TEST_DATA_DIR "${CMAKE_CURRENT_LIST_DIR}/data")
message(STATUS "TEST_DATA_DIR: ${TEST_DATA_DIR}")
set(TEST_DATA_COPY_DIR "${CMAKE_BINARY_DIR}/sample/data")
message(STATUS "TEST_DATA_COPY_DIR: ${TEST_DATA_COPY_DIR}")
add_custom_command(
  TARGET ${PROJECT_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory "${TEST_DATA_DIR}" "${TEST_DATA_COPY_DIR}")

# 安装
install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_LIST_DIR}/data DESTINATION bin)
