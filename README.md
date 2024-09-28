# STM32F4 CMake Project

## Overview

This repository provides a CMake-based build system for STM32F4 microcontrollers. It utilizes the `libopencm3` library for peripheral drivers and offers a straightforward way to build, debug, and test STM32F4 projects.

## Features

- **CMake Integration**: Build system configuration using CMake.
- **libopencm3 Support**: Utilizes `libopencm3` for peripheral drivers and low-level functions.
- **ST-Link Debugging**: Supports debugging through ST-Link.
- **Unit Testing**: Unit tests can be added and run on Windows.
- **Command-Line Build**: Project build is managed through Make; use `make help` to see available commands.

## Prerequisites

- **CMake**: Ensure CMake is installed. Download it from [CMake's official website](https://cmake.org/download/).
- **ARM Toolchain**: Install an ARM toolchain such as `arm-none-eabi` or `arm-elf`. For instance, you can get it from [GCC ARM Embedded](https://launchpad.net/gcc-arm-embedded).
- **GNU Make for Windows**: Download and install GNU Make from [GnuWin32](http://gnuwin32.sourceforge.net/packages/make.htm).
- **Visual Studio Code (VSCode)**: Install VSCode from [Visual Studio Code](https://code.visualstudio.com/) for firmware development and debugging.
- **Visual Studio**: Install Visual Studio from [Visual Studio](https://visualstudio.microsoft.com/) for running unit tests.
- **Python**: Install Python from [Python.org](https://www.python.org/downloads/windows/). Any recent version should be suitable.

### When not using precompiled files

It is possible to build libopencm3 for any platform by yourself. Libopencm3 has makefiles for Linux platform - it is not possible to compile it directly on Windows. MSYS is needed with special settings.

- **MSYS**: Install MSYS for a Unix-like environment and command-line tools on Windows. Download it from [MSYS 1.0.11](http://sourceforge.net/projects/mingw/files/MSYS/Base/msys-core/msys-1.0.11/MSYS-1.0.11.exe).

## Getting Started

To get started with this project, follow these steps:

### Cloning the Repository

```bash
git clone https://github.com/BenesAdam/stm32f4-cmake.git
cd stm32f4-cmake
subst S: .
cd S:
```

Working on this project on substed drive **S** is recommended.

### Examples of makefile targets

1. Get list of all target in Makefile:

    ```bash
    make help
    ```

2. Configure and generate cmake:

    ```bash
    make generate
    ```

3. Compile final firmware:

    ```bash
    make compile
    ```

4. Run unit tests:

   ```bash
    make ut_run
    ```

5. Run desired target in cmake (for example help):

   ```bash
    make target t=help
    ```

6. Clean build folder:

    ```bash
    # Remove only build folder
    make remove
    # Remove build folder and external cloned via git
    make remove_all
    ```

### Debugging

ST-Link: You can debug the project using ST-Link. Ensure the ST-Link drivers are installed and configured on your system (also added to PATH). You can test it with following commands (make sure that ST-Link is connected to PC and STM32F4 is connected to ST-Link):

```bash
st-info.exe --version 
st-info.exe --probe 
```

For debugging there are two launch configurations for ST-Link:

1. ST-Link: Debug Application (build, flash and debug FW)
2. ST-Link: Attach to active (attach to already flashed and running FW)

### Unit Testing

Unit tests can be added to the project and run on Windows. Ensure you have CTest framework configured (vscode extension).

### Project Structure

The project directory is organized as follows:

### Directories

- **`.vs`**: Configuration for Visual Studio.

- **`.vscode`**: Configuration files for Visual Studio Code, including `launch.json` and `c_cpp_properties.json`.

- **`app`**: Contains the main application code. This directory includes the entry point for the application.

- **`build`**: Output directory where the build artifacts are generated.

- **`cmake`**: Contains supporting CMake scripts, including integration with `libopencm3`.

- **`docs`**: Documentation directory for the microcontroller and other components.

- **`external`**: Directory for external components, typically fetched from Git repositories. This includes source code for `libopencm3`.

- **`linker`**: Contains linker scripts used for linking the application.

- **`precompiled`**: Contains precompiled `libopencm3` binaries, which eliminates the need for configuring MSYS.

- **`src`**: Source code directory where each component is responsible for a specific functionality.

- **`tests`**: Directory for unit tests.

- **`toolchains`**: Contains toolchain files, such as those for `arm-none-eabi-gcc`.

- **`tools`**: Directory for various tools, such as those for generating configuration files.

### Files

- **`README.md`**: This file, containing information about the project.

- **`Makefile`**: Makefile with shortcuts for compilation, running unit tests, and other build-related tasks.

- **`CMakeLists.txt`**: Main CMake configuration file for the project.

### Acknowledgements

- [CMake Documentation](https://cmake.org/documentation/): Comprehensive resources on how to use CMake for building projects.
- [STM32F4 Reference Manuals](https://www.st.com/en/microcontrollers-microprocessors/stm32f4-series.html): Detailed reference manuals for the STM32F4 microcontroller series.
- [libopencm3 Documentation](https://libopencm3.org/docs/): Documentation for the `libopencm3` library, including usage and configuration guides.

## Conclusion

Embarking on a project with STM32F4 microcontrollers and CMake is a rewarding and engaging hobby. Whether you're designing complex embedded systems or simply experimenting with new ideas, this project aims to make your development process smoother and more enjoyable.

We hope you find this setup useful and that it enhances your exploration and creativity in the world of embedded systems. Enjoy diving into your projects, and happy coding!

Happy tinkering!
