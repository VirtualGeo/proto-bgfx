#! /bin/bash -e

if [ $# -ne 1 ]; then
    echo "usage : $0 shaderName"
    exit 1
fi

echo $1

shaderPath="D:/proto-bgfx/shaders"

if [ ! -e $shaderPath/src/$1.vert.sc ] || [ ! -e $shaderPath/src/$1.frag.sc ]; then
    echo "shader '$1' not found"
fi

profile=""
#profile="--platform vs_5_0"
platform="linux"
D:/bgfx.cmake/bgfx-install/bin/shaderc.exe -f $shaderPath/src/$1.vert.sc -o $shaderPath/bin/$1.vert.bin --platform $platform --type vertex --verbose $profile -i D:/proto-bgfx/external/bgfx/shaders/src/
D:/bgfx.cmake/bgfx-install/bin/shaderc.exe -f $shaderPath/src/$1.frag.sc -o $shaderPath/bin/$1.frag.bin --platform $platform --type fragment --verbose $profile -i D:/proto-bgfx/external/bgfx/shaders/src/

#cp -v $shaderPath/bin/dx11/* $shaderPath/bin/
