set -e
mkdir -p build
cd build
cmake ../
make app test
cd ..
