#!/bin/bash
CMAKE_PROJECT_NAME=ltr_tools
if [ -z "$1" ]; then
    echo "No argument supplied, exiting"
    exit
fi
newFileName=$(printf '%s' "${1%.ltr}_edited.ltr")
ltr2vtk_convertor "$1"
echo "Starting ParaView, edit your trajectory and save it to /tmp/traj.vtk"
paraview /usr/local/etc/${CMAKE_PROJECT_NAME}/paraview_config_edit.pvsm 2>&1 1>/dev/null
vtk2ltr_convertor map.vtk traj.vtk "$newFileName"
echo "Output saved to $newFileName"
