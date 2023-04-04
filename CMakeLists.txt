cmake_minimum_required(VERSION 2.8.3)
project(ltr_tools)

set(CMAKE_CXX_STANDARD 17)

find_package(libpointmatcher)
add_executable(ltr_convertor src/main.cpp)
target_link_libraries(ltr_convertor ${libpointmatcher_LIBRARIES})

message(${CMAKE_BINARY_DIR})

install(TARGETS ltr_convertor DESTINATION bin RENAME ltr_convertor)
install(PROGRAMS scripts/visualiseLtr.sh DESTINATION bin RENAME visualizeLtr)
install(FILES config/paraview_config.pvsm DESTINATION /usr/local/etc/${CMAKE_PROJECT_NAME})
