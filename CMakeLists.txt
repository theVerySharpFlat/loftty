cmake_minimum_required(VERSION 3.21)
project(terminal)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(terminal src/main.cpp src/Terminal.cpp src/Pty.cpp src/Emulator.cpp)
target_link_libraries(terminal
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
        util
        )

