# proto-bgfx

## Dependencies
* bgfx, bx, bimg (system library)
* glfw (submodule)
* tinyobjloader (submodule)

---

## Building (command line instruction)
```bash
$ git submodule init
$ git submodule update
$ mkdir build
$ cd build
$ cmake ..
$ make
```

## How to install bgfx, bx, bimg library
```bash
$ git clone https://github.com/JoshuaBrookover/bgfx.cmake.git
$ cd bgfx.cmake
$ git submodule init
$ git submodule update
```

add line in root CMakeLists.txt
set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR}/bgfx-install)

add line in cmake/3rdparty/astc-codec.cmake
set_target_properties( astc-codec PROPERTIES DEBUG_POSTFIX d)

run these commands to generate all the bgfx-install library in the root of bgfx.cmake repository
```bash
$ cmake --build . --target install --config Debug
$ cmake --build . --target install --config Release
```

## How to include bgfx, bx, bimg library into proto-bgfx project
you must set the cmake variable BGFX_ROOT as the path of bgfx-install on your system

### Visual Studio Code example
in .vscode/settings.json of proto-bgfx project add these lines
```json
"cmake.configureSettings": {
        "BGFX_ROOT": "D:/bgfx.cmake/bgfx-install",
},
```
