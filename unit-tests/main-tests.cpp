#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "cgj-math/vec.hpp"

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
