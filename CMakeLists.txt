cmake_minimum_required(VERSION 3.16)

project(MediaPlayer VERSION 0.1 LANGUAGES CXX)

# Qt Setup
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt modules via find_package
find_package(Qt6 REQUIRED COMPONENTS Core Gui Qml Quick QuickControls2)

# OpenCV manual setup (no find_package)
set(OPENCV_DIR "C:/opencv/build")
link_directories(${OPENCV_DIR}/x64/vc16/lib)
set(OPENCV_LIBS opencv_world480)

# GStreamer manual setup (Windows)
set(GSTREAMER_DIR "C:/gstreamer/1.0/msvc_x86_64")
link_directories(${GSTREAMER_DIR}/lib)
set(GSTREAMER_LIBS
    gstreamer-1.0
    gstapp-1.0
    gstvideo-1.0
    gstbase-1.0
    gobject-2.0
    glib-2.0
    gstpbutils-1.0
)

file(GLOB_RECURSE CPP_SOURCES src/*.cpp)
file(GLOB_RECURSE CPP_HEADERS include/*.h)
file(GLOB_RECURSE QRC_FILES resource/*.qrc)

# Main executable
qt_add_executable(MediaPlayer ${CPP_SOURCES} ${CPP_HEADERS} ${QRC_FILES})

# Include headers
target_include_directories(MediaPlayer PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${OPENCV_DIR}/include
    ${GSTREAMER_DIR}/include/gstreamer-1.0
    ${GSTREAMER_DIR}/include/glib-2.0
    ${GSTREAMER_DIR}/lib/glib-2.0/include
)

# Link libraries
target_link_libraries(MediaPlayer
    PRIVATE
        Qt6::Core
        Qt6::Gui
        Qt6::Qml
        Qt6::Quick
        Qt6::QuickControls2
        ${OPENCV_LIBS}
        ${GSTREAMER_LIBS}
)
