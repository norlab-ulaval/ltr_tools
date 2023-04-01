#!/bin/bash
CMAKE_PROJECT_NAME=ltr_tools

ltr_convertor "$1"
echo "Starting ParaView..."
paraview /usr/local/etc/${CMAKE_PROJECT_NAME}/paraview_config.pvsm 2>&1 1>/dev/null