#include "ResourceNetwork.h"
#include <limits>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>

ResourceNetwork::ResourceNetwork()
{
  Reset();
}

ResourceNetwork::Graph::vertex_descriptor ResourceNetwork::AddProducer()
{
  Graph::vertex_descriptor v0 = boost::add_vertex(g);
  mProducers.push_back(v0);
  boost::add_edge(mMainProducer, v0, std::numeric_limits<PropertyType>::max(), g);
  return v0;
}

ResourceNetwork::Graph::vertex_descriptor ResourceNetwork::AddConsumer()
{
  Graph::vertex_descriptor v0 = boost::add_vertex(g);
  mConsumers.push_back(v0);
  boost::add_edge(v0, mMainConsumer, std::numeric_limits<PropertyType>::max(), g);
  return v0;
}

void ResourceNetwork::AddTransmitter(Graph::vertex_descriptor from, Graph::vertex_descriptor to, PropertyType weight)
{
  boost::add_edge(from, to, weight, g);
}

ResourceNetwork::PropertyType ResourceNetwork::MaxFlow()
{
  PropertyType flow = boykov_kolmogorov_max_flow(g, mMainProducer, mMainConsumer);
  return flow;
}

void ResourceNetwork::Reset()
{
  g.clear();

  mMainProducer = boost::add_vertex(g);
  mMainConsumer = boost::add_vertex(g);
}
