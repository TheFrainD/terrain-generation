project(viewer)

file(GLOB_RECURSE ${PROJECT_NAME}_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
file(GLOB_RECURSE ${PROJECT_NAME}_HEADERS ${CMAKE_CURRENT_SOURCE_DIR}/include/*.h)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES} ${${PROJECT_NAME}_HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_compile_definitions(${PROJECT_NAME} PRIVATE GLFW_INCLUDE_NONE)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(stb REQUIRED)

target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW::GLEW spdlog::spdlog glm::glm stb::stb)

# Compile GLSL shaders to SPIR-V

file(GLOB_RECURSE ${PROJECT_NAME}_GLSL_SOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert
  ${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag)

foreach(GLSL ${${PROJECT_NAME}_GLSL_SOURCES})
  get_filename_component(FILE_NAME ${GLSL} NAME)
  set(SPIRV ${PROJECT_BINARY_DIR}/shaders/${FILE_NAME}.spv)
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${CMAKE_COMMAND} -E make_directory
    ${PROJECT_BINARY_DIR}/shaders
    COMMAND glslangValidator -G ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARIES ${SPIRV})
endforeach(GLSL)

add_custom_target(shaders DEPENDS ${SPIRV_BINARIES})

add_dependencies(${PROJECT_NAME} shaders)

# Install application

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(DIRECTORY ${PROJECT_BINARY_DIR}/shaders DESTINATION ${CMAKE_SOURCE_DIR}/bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/textures DESTINATION ${CMAKE_SOURCE_DIR}/bin)
