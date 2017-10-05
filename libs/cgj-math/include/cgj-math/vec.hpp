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
    Vec(const Vec &copyFrom);

    // Sum with given values or vector
    Vec & sum(float a);
    Vec & sum(float a, float b);
    Vec & sum(float a, float b, float c);
    Vec & sum(float a, float b, float c, float d);
    Vec & sum(Vec &with);

    // Scalar multiplication
    Vec & scale(float a);

    // Dot product
    float dot(Vec &with);

    // Quadrance
    float quadrance();

    // Norm
    float norm();

    // Normalise
    Vec & normalise();

    // Cross product
    Vec & cross(Vec &with);

    // Projection
    Vec & proj(Vec on);

    // Angle
    float angle(Vec &with);

    // Rotate
    Vec & rotate(Vec &quaternion);

};
