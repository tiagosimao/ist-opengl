class Vec
{
  public:
    float a;
    float b;
    float c;
    float d;

    Vec();
    Vec(float a);
    Vec(float a, float b);
    Vec(float a, float b, float c);
    Vec(float a, float b, float c, float d);

    // Sum with given values or vector
    Vec & sum(float a);
    Vec & sum(float a, float b);
    Vec & sum(float a, float b, float c);
    Vec & sum(float a, float b, float c, float d);
    Vec & sum(Vec with);

    // Scalar multiplication
    Vec & scale(float a);

    // Dot product
    float dot(Vec with);

};
