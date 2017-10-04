1. How to build
  This project uses cmake and therefore should be buildable in MacOS, Linux or Windows.
  The build.sh script tries to help out with this by running cmake and make in the correct locations

  Install CMake https://cmake.org/download/

  run build.sh

  The built output should appear in bin/

2. How to run
  run.sh

3. How to test
  test.sh
  The tests output log should appear in bin/Testing/Temporary/*log

4. Vector implementation considerations
  Header: libs/cgj-math/include/cgj-math/vec.hpp
  Implementation: libs/cgj-math/src/vec.cpp
  Tests: unit-tests/main-tests.cpp

  A single class (Vec) was implemented.
  The major focus was performance (low memory allocation + low instruction count)
  and therefore a pure standard OOP approach was avoided, details bellow.

  4.1) Always four dimensions
    Overloaded constructors simplify usage by allowing instantiation of vectors with less dimensions
    Non-defined dimension's value defaults to zero
    Simplicity: The same API for 1D, 2D, 3D and 4D vector objects
    Agility: Possible to run operations between vectors with different dimensions
    Completeness: an nD implementation would probably be an overkill as dynamically
      sized structures are more complex to implement and expensive to run.
      Also 4 dimensions should be the maximum required number for a 3D game engine (queternions over 3D space?)

  4.2) Publicly accessible fields
    Each of the vector values (a, b, c and d) are publicly available fields
    Performance: Vec objects will be heavily used in each render, using getters and setters
      would potentially require more CPU instructions to achieve the same goal
    Legibility: Arguably someFunction(myVec.a, myVec.b) has better than someFunction(myVec.getA(), myVec.getB())

  4.3) Always floats
    Each of the vector values (a, b, c and d) are declared as floats
    Performance: float operations run directly on the CPU/GPU

  4.4) Mutable objects
    Running an operation over a vector will alter it's value
      vector1.sum(3,3); // this increment the a and b values of vector1 by 3
    Performance: Although more error prone, having mutable objects allow for
      low memory footprint and minimise object allocation. This might be counter-intuitive
      for operations like cross product or normalise.

  4.5) Method chaining
    Vector operations return the vector over which the operation occurred, allowing subsequent operations
      vector1.sum(1,0).sum(vector3).sum(3,3);
    Operations that don't alter the vector (like dot product) will break the chain
    and return something that is not a vector (like a float)
