#define BOOST_TEST_MODULE Flow Test
#include <boost/test/included/unit_test.hpp>

BOOST_AUTO_TEST_CASE(first_test)
{
  int i = 2;
  BOOST_TEST(i);
    BOOST_TEST(i == 2);
}