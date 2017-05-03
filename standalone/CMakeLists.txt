cmake_minimum_required (VERSION 3.1)
project (hiddenstar/standalone)

add_executable (hyper-sa hyper.c)
set_property (TARGET hyper-sa PROPERTY C_STANDARD 99)
target_link_libraries (hyper-sa ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

add_executable (planet-sa planet.c)
set_property (TARGET planet-sa PROPERTY C_STANDARD 99)
target_link_libraries (planet-sa planets ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

add_executable (translatorator script_gen.c)
add_executable (script_gen ALIAS translatorator)
set_property (TARGET translatorator PROPERTY C_STANDARD 99)

add_executable (tunnel-sa tunnel.c)
set_property (TARGET tunnel-sa PROPERTY C_STANDARD 99)
target_link_libraries (tunnel-sa ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES} m)

install (TARGETS hyper-sa DESTINATION hs/standalone OPTIONAL)
install (TARGETS planet-sa DESTINATION hs OPTIONAL)
install (TARGETS translatorator DESTINATION hs/standalone OPTIONAL)
install (TARGETS tunnel-sa DESTINATION hs/standalone OPTIONAL)
install (FILES spin.html DESTINATION hs/standalone OPTIONAL)
