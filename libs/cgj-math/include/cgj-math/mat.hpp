class Mat
{
  public:
    float m[16] = {
  	   1.0f,  0.0f,  0.0f, 0.0f,
  	   0.0f,  1.0f,  0.0f, 0.0f,
  	   0.0f,  0.0f,  1.0f, 0.0f,
  	   0.0f,  0.0f,  0.0f, 1.0f
     };
    Mat();
    Mat(const Mat &copyFrom);

    Mat scale(float a);

    Mat & move(float x, float y, float z);

    Mat mul(Mat &with);

    Mat rotX(float angle);

    Mat rotY(float angle);

    Mat rotZ(float angle);

    float * toGl();
};
