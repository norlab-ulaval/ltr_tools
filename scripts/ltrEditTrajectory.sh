#!/bin/bash
CMAKE_PROJECT_NAME=ltr_tools
if [ -z "$1" ]; then
    echo "No argument supplied, exiting"
    exit
fi
newFileName=$(printf '%s' "${1%.ltr}_edited.ltr")
ltr2vtk_convertor "$1"
editedTrajFileName="/tmp/traj.csv"
echo "Starting ParaView, edit your trajectory and save it to $editedTrajFileName"
paraview /usr/local/etc/${CMAKE_PROJECT_NAME}/paraview_config_edit.pvsm 2>&1 1>/dev/null
if [ -z "$editedTrajFileName" ]; then
  echo "$editedTrajFileName missing, exiting"
  exit
fi
# remove all quotes in the first column of the csv file and rename coordinates to xyz
sed -i  -e 's/"//g' -e 's/Points:0/x/g' -e 's/Points:1/y/g' -e 's/Points:2/z/g' $editedTrajFileName
vtk2ltr_convertor /tmp/map.vtk "$editedTrajFileName" "$newFileName"
echo "Output saved to $newFileName"
