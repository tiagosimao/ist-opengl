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

Vec & Vec::sum(float a)
{
  return this->sum(a, 0, 0, 0);
}

Vec & Vec::sum(float a, float b)
{
  return this->sum(a, b, 0, 0);
}

Vec & Vec::sum(float a, float b, float c)
{
  return this->sum(a, b, c, 0);
}

Vec & Vec::sum(float a, float b, float c, float d)
{
  this->a+=a;
  this->b+=b;
  this->c+=c;
  this->d+=d;
  return *this;
}

Vec & Vec::sum(Vec with)
{
  return this->sum(with.a, with.b, with.c, with.d);
}
