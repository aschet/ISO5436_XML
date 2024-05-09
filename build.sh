# !/bin/sh

mkdir build
cd build
cmake ..
cmake --build .
cpack
mv -f openGPS-*-Linux*.tar.gz ..
cd ..