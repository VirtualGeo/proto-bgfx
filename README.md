# proto-bgfx

## Dependencies
* bgfx, bx, bimg (system libraries)
* glfw (submodule)
* tinyobjloader (submodule)

## Building (command line instruction)
```bash
git clone --recurse-submodules -j4 https://github.com/VirtualGeo/proto-bgfx.git
mkdir build
cd build
cmake ..
make -j4
```

## Run executable (in build directory)
```bash
src/proto-bgfx
```

---

## How to install bgfx, bx, bimg libraries
```bash
git clone https://github.com/JoshuaBrookover/bgfx.cmake.git
cd bgfx.cmake
rm -rf bgfx bimg bx
git clone git://github.com/bkaradzic/bx.git
git clone git://github.com/bkaradzic/bimg.git
git clone git://github.com/bkaradzic/bgfx.git
```

### Add line in root CMakeLists.txt
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bgfx-install)

### Add line in cmake/3rdparty/astc-codec.cmake
set_target_properties( astc-codec PROPERTIES DEBUG_POSTFIX d)

### Run these commands to generate all the bgfx-install library in the bgfx.cmake repository
```bash
cmake --build <BGFX_CMAKE_BUILD_DEBUG_DIR> --target install
cmake --build <BGFX_CMAKE_BUILD_RELEASE_DIR> --target install
```

## How to include bgfx, bx, bimg library into proto-bgfx project
You must set the cmake variable BGFX_ROOT as the path of bgfx-install on your system

### System Environment (linux) example
add this line into /etc/environment file
```bash
BGFX_ROOT="<PATH_IN_YOUR_SYSTEM>/bgfx-install"
```

### Visual Studio Code example
in .vscode/settings.json of proto-bgfx project add these lines
```json
"cmake.configureSettings": {
        "BGFX_ROOT": "D:/<PATH_IN_YOUR_SYSTEM>/bgfx-install",
},
```
