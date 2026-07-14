VCPKG_ROOT ?= $(HOME)/vcpkg
TOOLCHAIN = -DCMAKE_TOOLCHAIN_FILE=$(VCPKG_ROOT)/scripts/buildsystems/vcpkg.cmake

compiled_commands:
	cmake $(TOOLCHAIN) -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build .

build-release:
	cmake $(TOOLCHAIN) -DCMAKE_BUILD_TYPE=Release -B build .

build-debbug:
	cmake $(TOOLCHAIN) -DCMAKE_BUILD_TYPE=Debug -B build .

compile:
	cmake --build build -j$(nproc)

run:
	cd build && ./soVIMter

clean:
	rm -rf build build-export output

fast-run-debug: build-debbug compile run

# Release build packaged into <repo>/output for distribution
export:
	cmake $(TOOLCHAIN) -DCMAKE_BUILD_TYPE=Release -DEXPORT_BUILD=ON -B build-export .
	cmake --build build-export -j$(nproc)

.PHONY: compiled_commands build-release build-debbug compile run clean fast-run-debug export
