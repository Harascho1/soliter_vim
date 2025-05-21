cmake -DCMAKE_BUILD_TYPE=Release -B build .
#cmake -B build .
cmake --build build -j$(nproc)
cd output
./soVIMter
cd ..
