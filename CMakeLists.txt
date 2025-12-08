cmake_minimum_required(VERSION 3.16)
project(Cinema LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Cinema
        Source/Main.cpp
        Source/Util.cpp
        Source/glad.c

        Header/Util.h
        Header/glad/glad.h
        Header/KHR/khrplatform.h
        Header/stb_image.h
        Header/Seat.h
        Header/SeatGrid.h
        Header/Door.h
        Header/Person.h
        Header/PeopleManager.h
        Header/Screen.h
        Header/State.h
        Header/CursorManager.h
        Source/Seat.cpp
        Source/SeatGrid.cpp
        Source/Door.cpp
        Source/Person.cpp
        Source/PeopleManager.cpp
        Source/Screen.cpp
        Source/CursorManager.cpp
        Header/Renderers/SeatRenderer.h
        Renderers/SeatRenderer.cpp
        Renderers/SignatureRenderer.cpp
        Header/Renderers/SignatureRenderer.h
)

# header folderi
target_include_directories(Cinema PRIVATE
        ${CMAKE_SOURCE_DIR}/Header
        ${CMAKE_SOURCE_DIR}/Header/glad
        ${CMAKE_SOURCE_DIR}/Header/KHR
        ${CMAKE_SOURCE_DIR}/Resources
        ${CMAKE_SOURCE_DIR}/Shaders
        ${CMAKE_SOURCE_DIR}/Source
)

# GLFW cross-platform
find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Cinema PRIVATE glfw)

# macOS
if(APPLE)
    target_link_libraries(Cinema PRIVATE
            "-framework Cocoa"
            "-framework IOKit"
            "-framework CoreVideo"
            "-framework OpenGL"
    )
endif()
