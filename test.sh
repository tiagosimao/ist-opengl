set -e
mkdir -p bin
cd bin
cmake ../
make all test
cd ..
