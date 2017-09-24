set -e
mkdir -p build
## g++ /usr/local/lib/libGLEW.dylib -v -I/usr/local/Cellar/glew/2.1.0/include -framework OpenGL starting-opengl.cpp -o bin/go
cd build
cmake ../
make
cd ..
./build/app
