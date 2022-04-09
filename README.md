[![CMake](https://github.com/cracked-machine/embedded_utils/actions/workflows/cmake.yml/badge.svg)](https://github.com/cracked-machine/embedded_utils/actions/workflows/cmake.yml)
[![Codecov](https://img.shields.io/codecov/c/github/cracked-machine/embedded_utils)](https://app.codecov.io/gh/cracked-machine/embedded_utils)


See the [wiki](https://github.com/cracked-machine/embedded_utils/wiki) for documentation / reference

See [readme](tests) for information on unit testing/mocking.

#### Adding this library to your STM32 Project

Include this repo into your project as a submodule and add the following line to your top-level CMakeFiles.txt:

`add_subdirectory(embedded_utils)`

This assumes your top-level CMakeFiles.txt is already configured for STM32 platform.

