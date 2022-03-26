[![CMake](https://github.com/cracked-machine/embedded_utils/actions/workflows/cmake.yml/badge.svg)](https://github.com/cracked-machine/embedded_utils/actions/workflows/cmake.yml)
https://img.shields.io/codecov/c/github/cracked-machine/embedded_utils

See the [wiki](https://github.com/cracked-machine/embedded_utils/wiki) for documentation / reference

See `.vscode/tasks.json` for details on the individual toolchain commands.
#### Running Units Tests on X86

When you run the default CMake build, the output is linked with the Catch2 library. To run the testsuite use the command:
`./build/test_suite`


#### Adding this library to your STM32 Project

Include this repo into your project as a submodule and add the following line to your top-level CMakeFiles.txt:

`add_subdirectory(embedded_utils)`

This assumes your top-level CMakeFiles.txt is already configured for STM32 platform.

