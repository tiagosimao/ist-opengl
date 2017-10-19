set -e
mkdir -p bin
cd bin
cmake ../
make
cd ..
./bin/main-tests
