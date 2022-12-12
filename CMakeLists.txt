cmake_minimum_required(VERSION 3.14)

project(button VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

#SET(CMAKE_CXX_FLAGS -I/usr/include/json-c)
SET(CMAKE_CXX_FLAGS -I/usr/local/include/json-c)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-fpermissive)
set(OPENCV_LOG_LEVEL ERROR)
find_package(OpenCV REQUIRED)
#find_package(json-c CONFIG)

#set(INCLUDE_PATH "/home/kiencate/ncnn/build/install")
set(INCLUDE_PATH "/home/linaro/ncnn/build/install")

set(EXTRA_LIBS ${OpenCV_LIBS} ${INCLUDE_PATH}/lib/libncnn.a)
include_directories(include ${OpenCV_INCLUDE_DIRS} ${INCLUDE_PATH}/include/ncnn)
add_link_options(-fopenmp -s -ldl -lpthread)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Quick DBus)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Quick DBus)
#-L/home/kiencate/Qt/5.15.2/gcc_64/lib -lQt5DBus
set(PROJECT_SOURCES
        main.cpp
        qml.qrc
        backend.cpp backend.h
        agent.cpp agent.h
        serversocket.cpp serversocket.h
        dbusadaptor.cpp dbusadaptor.h
        opencvimageprovider.cpp opencvimageprovider.h
        videostreamer.cpp videostreamer.h
        checkstatus.cpp checkstatus.h
        touchevent.cpp touchevent.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(button
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET button APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(button SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(button
          ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(button
  PRIVATE Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Quick Qt${QT_VERSION_MAJOR}::DBus ${EXTRA_LIBS} ${OpenCV_LIBS}  -lzbar -lbluetooth)
set_target_properties(button PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
target_link_libraries(button PRIVATE json-c)
install(TARGETS button
    DESTINATION /home/linaro/Rfid_rc522_i2c_linux/build
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_import_qml_plugins(button)
    qt_finalize_executable(button)
endif()
