cmake_minimum_required(VERSION 3.2.2)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})
endforeach()



### Main project.
project(RaspClock VERSION 1.0.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_REQUIRED TRUE)

set (SOURCES
    src/core/application_updater.cpp
    src/core/application_updater.h
    src/core/config.cpp
    src/core/config.h
    src/core/logs_recorder.cpp
    src/core/logs_recorder.h
    src/core/main_timer.cpp
    src/core/main_timer.h
    src/core/network.cpp
    src/core/network.h
    src/data/time_provider.cpp
    src/data/time_provider.cpp
    src/data/time_zone_provider.cpp
    src/data/time_zone_provider.h
    src/main.cpp
    src/ui/display.cpp
    src/ui/display.h
    src/ui/display.ui
    src/ui/logs.cpp
    src/ui/logs.h
    src/ui/logs.ui
    src/ui/main_window.cpp
    src/ui/main_window.h
    src/ui/main_window.ui
    src/ui/settings.cpp
    src/ui/settings.h
    src/ui/settings.ui
    src/ui/util/text_helpers.cpp
    src/ui/util/text_helpers.h
    src/ui/util/widget_ratio_filter.cpp
    src/ui/util/widget_ratio_filter.h
)

add_executable(raspclock WIN32 ${SOURCES})
target_include_directories(raspclock PRIVATE src)
set_target_properties(raspclock PROPERTIES AUTOMOC ON AUTOUIC ON)
if (NOT DEFINED CI_BUILD_ID)
    set (CI_BUILD_ID "dev")
endif()
target_compile_definitions(raspclock PRIVATE CI_BUILD_ID=${CI_BUILD_ID})



### Qt5

# Set Qt5_DIR variable to make this work: (e.g. Qt\5.15.1\msvc2019_64\lib\cmake\Qt5)
find_package(Qt5 COMPONENTS Widgets Network REQUIRED)
target_link_libraries(raspclock Qt5::Core Qt5::Gui Qt5::Widgets Qt5::Network)

macro(copy_qt5_dll COMPONENT)
    add_custom_command(
        TARGET raspclock POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt5::${COMPONENT}> ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
    )
endmacro()
copy_qt5_dll(Widgets)
copy_qt5_dll(Network)
copy_qt5_dll(Core)
copy_qt5_dll(Gui)

if (WIN32)
    add_custom_command(
        TARGET raspclock POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/platforms/"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/platforms/"
    )
endif()
