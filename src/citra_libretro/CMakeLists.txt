set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/CMakeModules)

add_library(citra2018_libretro SHARED
        emu_window/libretro_window.cpp
        emu_window/libretro_window.h
        input/input_factory.cpp
        input/input_factory.h
        input/mouse_tracker.cpp
        input/mouse_tracker.h
        citra_libretro.cpp
        citra_libretro.h
        environment.cpp
        environment.h
        core_settings.cpp
        core_settings.h
        libretro_logger.cpp
        libretro_logger.h)

create_target_directory_groups(citra2018_libretro)

target_link_libraries(citra2018_libretro PRIVATE common core)
target_link_libraries(citra2018_libretro PRIVATE glad libretro)
target_link_libraries(citra2018_libretro PRIVATE ${PLATFORM_LIBRARIES} Threads::Threads)
target_link_libraries(citra2018_libretro PRIVATE -static-libstdc++)

set_target_properties(citra2018_libretro PROPERTIES PREFIX "")
