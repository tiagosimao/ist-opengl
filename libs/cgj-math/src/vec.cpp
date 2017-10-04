#include "../include/cgj-math/vec.hpp"
#include <math.h>

Vec::Vec()
{
  this->a=0;
  this->b=0;
  this->c=0;
  this->d=0;
}

Vec::Vec(float a)
{
  this->a=a;
  this->b=0;
  this->c=0;
  this->d=0;
}

Vec::Vec(float a, float b)
{
  this->a=a;
  this->b=b;
  this->c=0;
  this->d=0;
}

Vec::Vec(float a, float b, float c)
{
  this->a=a;
  this->b=b;
  this->c=c;
  this->d=0;
}

Vec::Vec(float a, float b, float c, float d)
{
  this->a=a;
  this->b=b;
  this->c=c;
  this->d=d;
}

Vec::Vec(const Vec &copyFrom)
{
  this->a=copyFrom.a;
  this->b=copyFrom.b;
  this->c=copyFrom.c;
  this->d=copyFrom.d;
}

// SUM
Vec & Vec::sum(float sumA)
{
  return this->sum(sumA, 0, 0, 0);
}

Vec & Vec::sum(float sumA, float sumB)
{
  return this->sum(sumA, sumB, 0, 0);
}

Vec & Vec::sum(float sumA, float sumB, float sumC)
{
  return this->sum(sumA, sumB, sumC, 0);
}

Vec & Vec::sum(float sumA, float sumB, float sumC, float sumD)
{
  a+=sumA;
  b+=sumB;
  c+=sumC;
  d+=sumD;
  return *this;
}

Vec & Vec::sum(Vec &with)
{
  return this->sum(with.a, with.b, with.c, with.d);
}

// SCALE
Vec & Vec::scale(float scale)
{
  a*=scale;
  b*=scale;
  c*=scale;
  d*=scale;
  return *this;
}

// DOT
float Vec::dot(Vec &with)
{
  return a*with.a + b*with.b + c*with.c + d*with.d;
}

// QUADRANCE
float Vec::quadrance()
{
  return a*a + b*b + c*c + d*d;
}

// NORM
float Vec::norm()
{
  return sqrt(quadrance());
}

// NORMALISE
Vec & Vec::normalise()
{
  float norm = this->norm();
  if(norm==0){
    return *this;
  } else {
    a/=norm;
    b/=norm;
    c/=norm;
    d/=norm;
    return *this;
  }
}

// CROSS PRODUCT
Vec & Vec::cross(Vec &with)
{
  float newA = this->b*with.c - this->c*with.b;
  float newB = this->c*with.a - this->a*with.c;
  float newC = this->a*with.b - this->b*with.a;
  a = newA;
  b = newB;
  c = newC;
  d = 0;
  return *this;
}

// Projection
Vec & Vec::proj(Vec on)
{
  on.normalise();
  on.scale(this->dot(on));
  a=on.a;
  b=on.b;
  c=on.c;
  d=on.d;
  return *this;
}

// Angle
float Vec::angle(Vec &with)
{
  Vec aThis(*this);
  Vec bThis(with);
  float dot = aThis.dot(bThis);
  float norms = aThis.norm() * bThis.norm();
  return acos(dot/norms);
}
