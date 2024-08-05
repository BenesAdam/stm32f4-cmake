#-----------------------------------------------------------------------------+
# Make settings                                                               |
#-----------------------------------------------------------------------------+
MAKEFLAGS += --no-print-directory

#-----------------------------------------------------------------------------+
# Debug/Release settings                                                      |
#-----------------------------------------------------------------------------+
DEBUG ?= 1
BUILD_TYPE ?= $(if $(filter 1,$(DEBUG)),Debug,Release)
BUILD_TYPE_DEF_CMAKE ?= -DCMAKE_BUILD_TYPE=$(BUILD_TYPE)

#-----------------------------------------------------------------------------+
# Verbose settings                                                            |
#-----------------------------------------------------------------------------+
V ?= 0
VERBOSE_FLAG ?= $(if $(filter 1,$(V)),-v,)

#-----------------------------------------------------------------------------+
# Makefile generation                                                         |
#-----------------------------------------------------------------------------+
CMAKE_FILES := $(wildcard **/CMakeLists.txt) $(wildcard **/*.cmake)
CMAKE_GEN_MAKEFILE := build/Makefile

#-----------------------------------------------------------------------------+
# Toolchain settings                                                          |
#-----------------------------------------------------------------------------+
TOOLCHAIN_DEF_CMAKE ?= -DCMAKE_TOOLCHAIN_FILE=toolchains/toolchain-arm-none-eabi-gcc.cmake

#-----------------------------------------------------------------------------+
# Targets                                                                     |
#-----------------------------------------------------------------------------+
.PHONY: remove remove_all generate compile target clean help
default: help

# >----------------------------------------------------------------------------
remove:
	@echo "Removing files in build folder..."
	@rm -rf build/* build/.*
	@echo "Build folder cleaned."

# >----------------------------------------------------------------------------
remove_all: remove
	@echo "Removing files in external/libopencm3 folder..."
	@rm -rf external/libopencm3/* external/libopencm3/.*
	@echo "External libopencm3 folder cleaned."

# >----------------------------------------------------------------------------
generate: $(CMAKE_GEN_MAKEFILE)
	@echo "Build system generated."

# >----------------------------------------------------------------------------
compile: generate
	@echo "Compiling the project..."
	@cmake --build build/ -j$(shell nproc) $(VERBOSE_FLAG) --
	@echo "Compilation finished."

# >----------------------------------------------------------------------------
$(CMAKE_GEN_MAKEFILE): $(CMAKE_FILES)
	@echo "Generating CMake files..."
	@cmake -S . -B build/ -G Ninja $(BUILD_TYPE_DEF_CMAKE) $(TOOLCHAIN_DEF_CMAKE)
	@touch $(CMAKE_GEN_MAKEFILE)
	@echo "CMake files generated."

# >----------------------------------------------------------------------------
target:
ifdef t
	@echo "Running target $(t)..."
	@cmake --build build/ -j$(shell nproc) -t $(t) --
	@echo "Target $(t) executed."
else
	@echo "Error: No target specified."
	@echo "Please provide a target using the format: 'make target t=<target>'."
	@echo "For example: 'make target t=help'"
endif

# >----------------------------------------------------------------------------
clean:
	@echo "Cleaning up build artifacts..."
	@$(MAKE) target t=clean
	@echo "Clean up finished."

# >----------------------------------------------------------------------------
help:
	@echo "Available targets:"
	@echo "  remove                    Remove all files in the build folder"
	@echo "  remove_all                Remove all files in the build and libopencm3 folders"
	@echo "  generate                  Generate the build system"
	@echo "  compile                   Compile the project"
	@echo "  clean                     Clean up build artifacts"
	@echo "  target t=<your_target>    Execute a specified target"
	@echo "  help                      Show this help message"
