cmake_minimum_required(VERSION 3.16)

project(MediaPlayer VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 REQUIRED COMPONENTS Quick)

set(GSTREAMER_DIR "C:/gstreamer/1.0/x86_64")
set(OPENCV_DIR "C:/opencv")

include_directories(${OPENCV_DIR}/include)
include_directories(${GSTREAMER_DIR}/include/gstreamer-1.0)
include_directories(${GSTREAMER_DIR}/include/glib-2.0)
include_directories(${GSTREAMER_DIR}/lib/glib-2.0/include)

link_directories(${GSTREAMER_DIR}/lib)
link_directories(${OPENCV_DIR}/x64/vc16/lib)
set(GSTREAMER_LIBS gstreamer-1.0 glib-2.0 gobject-2.0 gstapp-1.0 gstvideo-1.0 gstbase-1.0)
set(OPENCV_LIBS opencv_core480 opencv_imgproc480 opencv_imgcodecs480 opencv_tracking480)

qt_add_executable(appMediaPlayer
    main.cpp
)

qt_add_qml_module(appMediaPlayer
    URI MediaPlayer
    VERSION 1.0
    QML_FILES Main.qml
    SOURCES Gstreamer/gstreamervideocapture.cpp Gstreamer/gstreamervideocapture.h
    RESOURCES icons/icons.qrc
    QML_FILES Screens/TopScreen/TopStartUpScreen.qml
    QML_FILES Screens/BottomScreen/BottomStartUpScreen.qml
    QML_FILES Screens/LeftScreen/LeftStartUpScreen.qml
    QML_FILES Screens/RightScreen/RightStartUpScreen.qml
    QML_FILES Screens/MainScreen/MainScreen.qml
    SOURCES videoitem.h videoitem.cpp
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appMediaPlayer PROPERTIES
#    MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appMediaPlayer
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appMediaPlayer
    PRIVATE Qt6::Quick ${GSTREAMER_LIBS} ${OPENCV_LIBS}
)

include(GNUInstallDirs)
install(TARGETS appMediaPlayer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
