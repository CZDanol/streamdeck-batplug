cmake_minimum_required(VERSION 3.20)

# Build tested on MSVC 2016 x64

project(streamdeck-batplug LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ===========================================
# Target setup
# ===========================================
set(target streamdeck-batplug)

# Qt setup
find_package(Qt6 COMPONENTS Core WebSockets Network REQUIRED CONFIG)
set(CMAKE_AUTOMOC ON)

set(target_directory "${PROJECT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${target_directory}/bin")

qt_add_executable(${target})
target_link_libraries(${target} PRIVATE Qt6::WebSockets Qt6::Network)
target_compile_definitions(${target} PRIVATE QT_DISABLE_DEPRECATED_BEFORE=0x060200)

# ===========================================
# Sources
# ===========================================
file(GLOB_RECURSE source_files CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
target_sources(${target} PRIVATE ${source_files})
target_include_directories(${target} PRIVATE "${PROJECT_SOURCE_DIR}/src")

# ===========================================
# QtStreamDeck
# ===========================================
file(GLOB_RECURSE streamdeckplugin_files CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/deps/qtstreamdeck/*.cpp")
target_sources(${target} PRIVATE ${streamdeckplugin_files})
target_include_directories(${target} PRIVATE "${PROJECT_SOURCE_DIR}/deps/qtstreamdeck")

# ===========================================
# Deployment
# ===========================================

# Run windeployqt
get_target_property(QMAKE_FILEPATH Qt6::qmake IMPORTED_LOCATION)

install(CODE "set(target_file \"$<TARGET_FILE:${target}>\")")
install(CODE "set(QMAKE_FILEPATH \"${QMAKE_FILEPATH}\")")
install(CODE "set(PROJECT_SOURCE_DIR \"${PROJECT_BASE_DIR}\")")
install(CODE "set(CMAKE_BINARY_DIR \"${CMAKE_BASE_DIR}\")")
install(SCRIPT "${PROJECT_SOURCE_DIR}/deploy.cmake")