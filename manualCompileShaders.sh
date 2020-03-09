#! /bin/bash -e

shaderPath="D:/proto-bgfx/shaders/"

D:/bgfx.cmake/bgfx-install/bin/shaderc.exe -f $shaderPath/src/cubes.vert.sc -o $shaderPath/bin/dx11/cubes.vert.bin --platform windows --type vertex --verbose --profile vs_5_0 -i D:/proto-bgfx/external/bgfx/shaders/src/ -O3
D:/bgfx.cmake/bgfx-install/bin/shaderc.exe -f $shaderPath/src/cubes.frag.sc -o $shaderPath/bin/dx11/cubes.frag.bin --platform windows --type fragment --verbose --profile ps_5_0 -i D:/proto-bgfx/external/bgfx/shaders/src/ -O3

cp -v $shaderPath/bin/dx11/* $shaderPath/bin/