compiled_commands:
	cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -B build .

build-release:
	cmake -DCMAKE_BUILD_TYPE=Release -B build .

build-debbug:
	cmake -B build .

compile:
	cmake --build build -j$(nproc)

run:
	./output/soVIMter
