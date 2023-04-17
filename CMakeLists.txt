cmake_minimum_required(VERSION 2.8.3)
project(ltr_tools)

set(CMAKE_CXX_STANDARD 17)

find_package(libpointmatcher)
add_executable(ltr2vtkConvertor src/ltr2vtk_convertor.cpp)
target_link_libraries(ltr_convertor ${libpointmatcher_LIBRARIES})

message(${CMAKE_BINARY_DIR})

install(TARGETS ltr2vtkConvertor DESTINATION bin RENAME ltr2vtkConvertor)
install(PROGRAMS scripts/inspectLtr.sh DESTINATION bin RENAME inspectLtr)
install(FILES config/paraview_config.pvsm DESTINATION /usr/local/etc/${CMAKE_PROJECT_NAME})
