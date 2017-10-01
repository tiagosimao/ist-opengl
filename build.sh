set -e
mkdir -p bin
cd bin
cmake ../
echo "Building project"
make
cd ..
