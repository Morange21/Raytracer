# Raytracer
CUDA-enabled 3D Raytracer

==============USAGE====================
CUDA
	Must enable CUDA_SEPARABLE_COMPILATION CMake option manually
	May have to build twice for linking to work
CPU
	To disable CUDA build when it is available on your device, set cmake variable TRY_BUILD_CUDA=OFF
	
To have reasonable render times, build in release mode.
to fill dependencies (excluding NVIDIA CUDA samples) pull submodules


================================================================================
The MIT License
--------------------------------------------------------------------------------
Copyright (c) 2014 - 2017 Joshua Noel

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
