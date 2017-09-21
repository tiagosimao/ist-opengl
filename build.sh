mkdir -p bin
g++ /usr/local/lib/libGLEW.dylib /usr/local/lib/libglut.dylib -v -I/usr/local/Cellar/glew/2.1.0/include -I/usr/local/Cellar/freeglut/3.0.0/include -framework OpenGL starting-opengl.cpp -o bin/go
./bin/go
