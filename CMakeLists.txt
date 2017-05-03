cmake_minimum_required (VERSION 3.1)
project (hiddenstar)
set (CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

# SDL2
find_package (PkgConfig REQUIRED)
pkg_check_modules (SDL2 REQUIRED sdl2)
pkg_check_modules (SDL2IMAGE REQUIRED SDL2_image>=2.0.0)
pkg_check_modules (SDL2MIXER REQUIRED SDL2_mixer>=2.0.0)
include_directories (${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS} ${SDL2MIXER_INCLUDE_DIRS})

# hiddenstar files
add_library (audio audio.c)
add_library (combat combat.c)
add_library (comms comms.c)
add_library (factions factions.c)
add_library (gui gui.c)
add_library (intro intro.c)
add_library (menu menu.c)
add_library (planets planets.c)
add_library (starmap starmap.c)
add_library (trade trade.c)
add_library (user_events user_events.c)
add_executable (hiddenstar main.c)
set_property (TARGET hiddenstar PROPERTY C_STANDARD 99)
include_directories (util)
target_link_libraries (hiddenstar audio combat comms factions gui intro menu planets starmap trade user_events)
target_link_libraries (hiddenstar vector)
target_link_libraries (hiddenstar ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} ${SDL2MIXER_LIBRARIES} m)

add_subdirectory (standalone)
add_subdirectory (util)

# Installation
install (TARGETS hiddenstar DESTINATION hs)
install (FILES hidden-star.png DESTINATION hs)
install (DIRECTORY audio DESTINATION hs)
install (DIRECTORY sprites DESTINATION hs)
install (DIRECTORY text DESTINATION hs)
