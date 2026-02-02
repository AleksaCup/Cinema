cmake_minimum_required(VERSION 3.16)
project(Cinema LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cinema
        Source/Main.cpp
        Source/Util.cpp
        Source/glad.c

        # Core classes
        Source/Camera.cpp
        Source/Seat.cpp
        Source/SeatGrid.cpp
        Source/Person.cpp
        Source/Simulation.cpp

        # Renderers
        Source/Renderers/CubeRenderer.cpp
#        Source/Renderers/ScreenRenderer.cpp
        Source/Renderers/SignatureRenderer.cpp
        Source/Renderers/RoomRenderer.cpp

        # Headers
        Header/Util.h
        Header/glad/glad.h
        Header/KHR/khrplatform.h
        Header/stb_image.h
        Header/Camera.h
        Header/Seat.h
        Header/SeatGrid.h
#        Header/Person.h
#        Header/Simulation.h
        Header/Renderers/CubeRenderer.h
#        Header/Renderers/ScreenRenderer.h
        Header/Renderers/SignatureRenderer.h
        Header/Renderers/RoomRenderer.h
#        Source/Person.cpp
#        Source/Simulation.cpp
        Header/Renderers/RoomRenderer.h
        Source/Renderers/RoomRenderer.cpp
        Source/Main.cpp
        Header/Util.h
)

# Header directories
target_include_directories(Cinema PRIVATE
        ${CMAKE_SOURCE_DIR}/Header
        ${CMAKE_SOURCE_DIR}/Header/glad
        ${CMAKE_SOURCE_DIR}/Header/KHR
        ${CMAKE_SOURCE_DIR}/Resources
        ${CMAKE_SOURCE_DIR}/Shaders
        ${CMAKE_SOURCE_DIR}/Source
)

# GLFW
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Cinema PRIVATE glfw)

# GLM (header-only library)
find_package(glm CONFIG REQUIRED)
target_link_libraries(Cinema PRIVATE glm::glm)

# macOS
if(APPLE)
    target_link_libraries(Cinema PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            "-framework OpenGL"
    )
endif()

# Windows
if(WIN32)
    target_link_libraries(Cinema PRIVATE opengl32)
endif()

# Linux
if(UNIX AND NOT APPLE)
    target_link_libraries(Cinema PRIVATE GL X11 pthread Xrandr Xi dl)
endif()