cmake_minimum_required(VERSION 2.8.3)
project(ltr_tools)

set(CMAKE_CXX_STANDARD 17)

find_package(libpointmatcher)
add_executable(ltr2vtk_convertor src/ltr2vtk_convertor.cpp)
target_link_libraries(ltr2vtk_convertor ${libpointmatcher_LIBRARIES})

add_executable(ltr_add_z_coord src/ltr_add_z_coord.cpp)
target_link_libraries(ltr_add_z_coord ${libpointmatcher_LIBRARIES})

add_executable(vtk2ltr_convertor src/vtk2ltr_convertor.cpp)
target_link_libraries(vtk2ltr_convertor ${libpointmatcher_LIBRARIES})

message(${CMAKE_BINARY_DIR})

install(TARGETS ltr2vtk_convertor DESTINATION bin)
install(TARGETS ltr_add_z_coord DESTINATION bin)
install(TARGETS vtk2ltr_convertor DESTINATION bin)
install(PROGRAMS scripts/ltrInspect.sh DESTINATION bin RENAME ltrInspect)
install(PROGRAMS scripts/ltrEditTrajectory.sh DESTINATION bin RENAME ltrEditTrajectory)
install(FILES config/paraview_config.pvsm DESTINATION /usr/local/etc/${CMAKE_PROJECT_NAME})
install(FILES config/paraview_config_edit.pvsm DESTINATION /usr/local/etc/${CMAKE_PROJECT_NAME})
