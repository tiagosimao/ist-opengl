#include "../include/cgj-math/vec.hpp"

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
  this->a+=sumA;
  this->b+=sumB;
  this->c+=sumC;
  this->d+=sumD;
  return *this;
}

Vec & Vec::sum(Vec with)
{
  return this->sum(with.a, with.b, with.c, with.d);
}

// SCALE
Vec & Vec::scale(float scale)
{
  this->a*=scale;
  this->b*=scale;
  this->c*=scale;
  this->d*=scale;
  return *this;
}

// DOT
float Vec::dot(Vec with)
{
  return this->a*with.a+this->b*with.b+this->c*with.c+this->d*with.d;
}
