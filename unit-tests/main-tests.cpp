#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "cgj-math/vec.hpp"
#include <math.h>
#include "spdlog/spdlog.h"

auto logger = spdlog::stdout_color_mt("main");

void calc(Vec view, Vec up)
{
  logger->info("view=({},{},{}), up=({},{},{})",view.a,view.b,view.c,up.a,up.b,up.c);
  if(up.angle(view)==0)
  {
    logger->info("\tup and view are collinear... ignoring\n");
    return;
  }

  Vec v(view);
  v.normalise();


  Vec w(up);
  w.cross(v).normalise();


  Vec u(v);
  u.cross(w);

  logger->info("\tv=({},{},{})",v.a,v.b,v.c);
  logger->info("\tu=({},{},{})",u.a,u.b,u.c);
  logger->info("\tw=({},{},{})\n",w.a,w.b,w.c);
}

/*
int main()
{
    Vec v1(1,0,0);
    Vec v2(0,2,0);
    Vec v3(0,0,3);

    calc(v1, v1);
    calc(v1, v2);
    calc(v1, v3);

    calc(v2, v1);
    calc(v2, v2);
    calc(v2, v3);

    calc(v3, v1);
    calc(v3, v2);
    calc(v3, v3);

}*/


TEST_CASE( "Constructors" ) {
  Vec v0;
  REQUIRE( v0.a==0 );
  REQUIRE( v0.b==0 );
  REQUIRE( v0.c==0 );
  REQUIRE( v0.d==0 );
  Vec v1(12);
  REQUIRE( v1.a==12 );
  REQUIRE( v1.b==0 );
  REQUIRE( v1.c==0 );
  REQUIRE( v1.d==0 );
  Vec v2(98,34);
  REQUIRE( v2.a==98 );
  REQUIRE( v2.b==34 );
  REQUIRE( v2.c==0 );
  REQUIRE( v2.d==0 );
  Vec v3(36,56,87);
  REQUIRE( v3.a==36 );
  REQUIRE( v3.b==56 );
  REQUIRE( v3.c==87 );
  REQUIRE( v3.d==0 );
  Vec v4(55,76,24,31);
  REQUIRE( v4.a==55 );
  REQUIRE( v4.b==76 );
  REQUIRE( v4.c==24 );
  REQUIRE( v4.d==31 );
}

TEST_CASE( "Assignment" ) {
  Vec v1(55,76,24,31);
  Vec v2(v1);
  REQUIRE( v2.a==55 );
  REQUIRE( v2.b==76 );
  REQUIRE( v2.c==24 );
  REQUIRE( v2.d==31 );
}

TEST_CASE( "Sum" ) {
  Vec v1;
  v1.sum(1);
  REQUIRE( v1.a==1 );
  REQUIRE( v1.b==0 );
  REQUIRE( v1.c==0 );
  REQUIRE( v1.d==0 );
  Vec v2(3);
  v2.sum(v1);
  REQUIRE( v1.a==1 );
  REQUIRE( v1.b==0 );
  REQUIRE( v1.c==0 );
  REQUIRE( v1.d==0 );
  REQUIRE( v2.a==4 );
  REQUIRE( v2.b==0 );
  REQUIRE( v2.c==0 );
  REQUIRE( v2.d==0 );

}

TEST_CASE( "Scalar Multiplication" ) {
  Vec v1(3,2);
  v1.scale(3);
  REQUIRE( v1.a==9 );
  REQUIRE( v1.b==6 );
  REQUIRE( v1.c==0 );
  REQUIRE( v1.d==0 );

  v1.scale(1);
  REQUIRE( v1.a==9 );
  REQUIRE( v1.b==6 );
  REQUIRE( v1.c==0 );
  REQUIRE( v1.d==0 );

}

TEST_CASE( "Dot Product" ) {
  Vec v1(4,-2);
  Vec v2(3,-1);
  REQUIRE( v1.dot(v2)==(4*3)+(-2*-1) );

  Vec v3(3,2,7,-3);
  Vec v4(-3,5,4,2);
  REQUIRE( v3.dot(v4)==(3*-3)+(2*5)+(7*4)+(-3*2));
}

TEST_CASE( "Quadrance" ) {
  Vec v1(3,2,7,-3);
  REQUIRE( v1.quadrance()==9+4+49+9);
}

TEST_CASE( "Norm" ) {
  Vec v1(3,2,7,-3);
  REQUIRE( v1.norm()==(float)sqrt(9+4+49+9));
}

TEST_CASE( "Normalisation" ) {
  Vec v1(3,6,6);
  Vec normalised(v1);
  normalised.normalise();

  REQUIRE( normalised.a==(float)(1./3) );
  REQUIRE( normalised.b==(float)(2./3) );
  REQUIRE( normalised.c==(float)(2./3) );
  REQUIRE( normalised.d==0 );

  REQUIRE( v1.a==3 );
  REQUIRE( v1.b==6 );
  REQUIRE( v1.c==6 );
  REQUIRE( v1.d==0 );
}

TEST_CASE( "Cross Product" ) {
  Vec v1(2,3,4);
  Vec v2(5,6,7);

  Vec crossed(v1);
  crossed.cross(v2);

  REQUIRE( crossed.a==-3 );
  REQUIRE( crossed.b==6 );
  REQUIRE( crossed.c==-3 );
  REQUIRE( crossed.d==0 );

  REQUIRE( v1.a==2 );
  REQUIRE( v1.b==3 );
  REQUIRE( v1.c==4 );
  REQUIRE( v1.d==0 );

  REQUIRE( v2.a==5 );
  REQUIRE( v2.b==6 );
  REQUIRE( v2.c==7 );
  REQUIRE( v2.d==0 );

}

TEST_CASE( "Projection" ) {
  Vec v1(2,3,4);
  Vec v2(1,0,0);

  v1.proj(v2);

  REQUIRE( v1.a==2 );
  REQUIRE( v1.b==0 );
  REQUIRE( v1.c==0 );
  REQUIRE( v1.d==0 );

  REQUIRE( v2.a==1 );
  REQUIRE( v2.b==0 );
  REQUIRE( v2.c==0 );
  REQUIRE( v2.d==0 );

  Vec v3(2,3,4);
  Vec v4(2,3,4);

  v3.proj(v4);

  REQUIRE( Approx(v3.a) == v4.a );
  REQUIRE( Approx(v3.b) == v4.b );
  REQUIRE( Approx(v3.c) == v4.c );
  REQUIRE( Approx(v3.d) == v4.d );

}

TEST_CASE( "Angle" ) {
  Vec v1(1,0);
  Vec v2(0,1);

  float angle1 = v1.angle(v2);
  REQUIRE( Approx(angle1) == M_PI/2 );

  Vec v3(3,4,7);
  Vec v4(1,-2,5);

  float angle2 = v3.angle(v4);
  REQUIRE( Approx(angle2) == 0.8805663586f);

}

/**

d           a       	  b        	  c        	  Description
1           0	          0        	  0        	  Identity quaternion, no rotation
0       	  1        	  0        	  0        	  180° turn around A axis
0       	  0        	  1        	  0        	  180° turn around B axis
0       	  0        	  0        	  1        	  180° turn around C axis
sqrt(0.5)	  sqrt(0.5)	  0       	  0        	  90° rotation around A axis
sqrt(0.5)	  0       	  sqrt(0.5)	  0        	  90° rotation around B axis
sqrt(0.5)	  0       	  0        	  sqrt(0.5)	  90° rotation around C axis
sqrt(0.5)	  -sqrt(0.5)  0       	  0        	  -90° rotation around A axis
sqrt(0.5)	  0       	  -sqrt(0.5)  0        	  -90° rotation around B axis
sqrt(0.5)  	0       	  0           -sqrt(0.5)  -90° rotation around C axis

**/

TEST_CASE( "Rotation" ) {
  Vec v1(1,0);
  Vec q1(0, 0, 0, 1);
  v1.rotate(q1);
  REQUIRE( v1.a==1 );
  REQUIRE( v1.b==0 );
  REQUIRE( v1.c==0 );
  REQUIRE( v1.d==0 );

  Vec v2(1,0);
  Vec q2(0,1,0,0);
  v2.rotate(q2);
  REQUIRE( v2.a== -1);
  REQUIRE( v2.b== 0);
  REQUIRE( v2.c== 0);
  REQUIRE( v2.d== 0);

  // 90º around C
  Vec v3(3,2,4);
  Vec q3(0,0,1,1);
  v3.rotate(q3);
  REQUIRE( Approx(v3.a) == -2);
  REQUIRE( Approx(v3.b) == 3);
  REQUIRE( v3.c== 4);
  REQUIRE( v3.d== 0);

  // 30º around C
  Vec v4(3,2,4);
  Vec q4(0, 0, sin(M_PI/12), cos(M_PI/12));
  v4.rotate(q4);
  REQUIRE( Approx(v4.a) == 1.598076);
  REQUIRE( Approx(v4.b) == 3.232051);
  REQUIRE( v4.c== 4);
  REQUIRE( v4.d== 0);

}


void testViewUp(Vec view, Vec up, int va, int vb, int vc, int ua, int ub, int uc, int wa, int wb, int wc)
{
  Vec v(view);
  v.normalise();

  Vec w(up);
  w.cross(v);
  if(w.norm()==0){
    REQUIRE_THROWS( w.normalise() );
    return;
  } else {
    w.normalise();
  }

  Vec u(v);
  u.cross(w);

  REQUIRE( v.a == va );
  REQUIRE( v.b == vb );
  REQUIRE( v.c == vc );

  REQUIRE( u.a == ua );
  REQUIRE( u.b == ub );
  REQUIRE( u.c == uc );

  REQUIRE( w.a == wa );
  REQUIRE( w.b == wb );
  REQUIRE( w.c == wc );
}

TEST_CASE( "view+up" ) {
  Vec v1(1,0,0);
  Vec v2(0,2,0);
  Vec v3(0,0,3);

  testViewUp(v1,v1,0,0,0,0,0,0,0,0,0);
  testViewUp(v1,v2,1,0,0,0,1,0,0,0,-1);
  testViewUp(v1,v3,1,0,0,0,0,1,0,1,0);
  testViewUp(v2,v1,0,1,0,1,0,0,0,0,1);
  testViewUp(v2,v2,0,0,0,0,0,0,0,0,0);
  testViewUp(v2,v3,0,1,0,0,0,1,-1,0,0);
  testViewUp(v3,v1,0,0,1,1,0,0,0,-1,0);
  testViewUp(v3,v2,0,0,1,0,1,0,1,0,0);
  testViewUp(v3,v3,0,0,0,0,0,0,0,0,0);

}
