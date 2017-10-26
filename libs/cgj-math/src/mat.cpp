#include "../include/cgj-math/mat.hpp"
#include <math.h>
#include <iostream>
#include <algorithm>

Mat::Mat()
{
}

Mat::Mat(const Mat &copyFrom)
{
  for(int i=0;i<16;++i){
    this->m[i]=copyFrom.m[i];
  }
}

Mat Mat::scale(float a)
{
  Mat scale;
  scale.m[15]=a==0?1:1/a;
  return this->mul(scale);
}

Mat& Mat::move(float x, float y, float z)
{
  this->m[3]+=x;
  this->m[7]+=y;
  this->m[11]+=z;
  return *this;
}

Mat & Mat::set(float values[16])
{
  for(int i=0;i<16;++i)
  {
    this->m[i]=values[i];
  }
  return *this;
}

Mat & Mat::set(int col, int row, float value)
{
  this->m[col + 4 * row] = value;
  return *this;
}

float Mat::at(int col, int row)
{
  return this->m[col + 4 * row];
}

Mat Mat::mul(Mat &with)
{
  Mat result;
  for(int aRow=0;aRow<4;++aRow) {
    result.m[0+4*aRow]=this->m[0+4*aRow]*with.m[0]+m[1+4*aRow]*with.m[4]+m[2+4*aRow]*with.m[8]+m[3+4*aRow]*with.m[12];
    result.m[1+4*aRow]=this->m[0+4*aRow]*with.m[1]+m[1+4*aRow]*with.m[5]+m[2+4*aRow]*with.m[9]+m[3+4*aRow]*with.m[13];
    result.m[2+4*aRow]=this->m[0+4*aRow]*with.m[2]+m[1+4*aRow]*with.m[6]+m[2+4*aRow]*with.m[10]+m[3+4*aRow]*with.m[14];
    result.m[3+4*aRow]=this->m[0+4*aRow]*with.m[3]+m[1+4*aRow]*with.m[7]+m[2+4*aRow]*with.m[11]+m[3+4*aRow]*with.m[15];
  }
  return result;
}

Mat Mat::rotX(float angle)
{
  Mat rot;
  rot.m[5]=cos(angle);
  rot.m[6]=-sin(angle);
  rot.m[9]=sin(angle);
  rot.m[10]=cos(angle);
  return this->mul(rot);
}

Mat Mat::rotY(float angle)
{
  Mat rot;
  rot.m[0]=cos(angle);
  rot.m[2]=sin(angle);
  rot.m[8]=-sin(angle);
  rot.m[10]=cos(angle);
  return this->mul(rot);
}

Mat Mat::rotZ(float angle)
{
  Mat rot;
  rot.m[0]=cos(angle);
  rot.m[1]=-sin(angle);
  rot.m[4]=sin(angle);
  rot.m[5]=cos(angle);
  return this->mul(rot);
}

float * Mat::toGl()
{
  return this->m;
}
