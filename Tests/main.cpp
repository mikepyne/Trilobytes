#define CATCH_CONFIG_RUNNER
#include "Libs/Catch2/catch.hpp"

int main( int argc, char* argv[] )
{
  return Catch::Session().run( argc, argv );
}
