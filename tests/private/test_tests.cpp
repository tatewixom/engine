#include <doctest.h>

int add(int x, int y)
{
  return x + y;
}


TEST_CASE("Addition works")
{
  CHECK(add(2, 3) == 5);
}