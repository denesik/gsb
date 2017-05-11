#define BOOST_TEST_MODULE Flow Test
#include <boost/test/included/unit_test.hpp>
#include <ResourceNetwork.h>

BOOST_AUTO_TEST_CASE(first_test)
{
  ResourceNetwork rn;
  auto prod1 = rn.AddProducer();
  auto cons1 = rn.AddConsumer();

  rn.AddTransmitter(prod1, cons1, 1);
  rn.AddTransmitter(prod1, cons1, 2);

  auto flow = rn.MaxFlow();
  BOOST_TEST(flow == 1);
}