# ltr_tools

Set of tools to work with .ltr files

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

## Uninstallation
```
sudo rm -rf /usr/local/bin/ltr2vtkConvertor /usr/local/bin/inspectLtr /usr/local/bin/ltr_add_z_coord /usr/local/etc/ltr_tools/
```


## Tools
### inspectLtr
Split an .ltr file into map and trajectory files and visualize them in ParaView with predefined window state.

#### Usage
```
inspectLtr <path_to_ltr_file>
```


### ltr_add_z_coord
Add the z coordinate to the first version of an .ltr file format, making it compatible with new code tools.
Z coordinate can either be explicitly set in the third argument, or is derived from each point's nearest neighbors in the map.

#### Usage
```
ltr_add_z_coord <path_to_input_ltr_file> <path_to_output_ltr_file> <z_coord>
```
