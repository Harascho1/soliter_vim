cmake -B build
cmake --build build -j$(nproc)
cd output
./soVIMter
cd ..