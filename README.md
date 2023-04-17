# ltr_tools

Simple tool to split an .ltr file into map and trajectory files and visualize them in ParaView with predefined window state.

## Dependencies
+ Eigen
+ libpointmatcher
+ ParaView

## Installation
```
cd ~/
git clone https://github.com/norlab-ulaval/ltr_tools.git
cd ltr_tools/
mkdir build && cd build/
cmake -DCMAKE_BUILD_TYPE=Release ..
make
sudo make install
```

## Usage
```
inspectLtr <path_to_ltr_file>
```

## Uninstallation
```
sudo rm -rf /usr/local/bin/ltr2vtk_convertor /usr/local/bin/inspectLtr /usr/local/etc/ltr_tools/
```