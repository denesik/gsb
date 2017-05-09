#pragma once
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/directed_graph.hpp>

class ResourceNetwork
{
public:
  ResourceNetwork();

  using ProrertySubtype = float;
  using PropertyType = std::pair<ProrertySubtype, ProrertySubtype>; // current, max
  using EdgeWeightProperty = boost::property<boost::edge_weight_t, PropertyType>;

  using Traits = boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    boost::property<boost::vertex_index_t, long>,
    boost::property<boost::edge_capacity_t, PropertyType>
  >;
  using V = Graph::vertex_descriptor;

  Graph g;

  Graph::vertex_descriptor AddProducer();
  Graph::vertex_descriptor AddConsumer();
  void AddTransmitter(V from, V to, ProrertySubtype weight);
  ProrertySubtype MaxFlow();

  void Reset();

  std::list<Graph::vertex_descriptor> mProducers;
  Graph::vertex_descriptor mMainProducer;

  std::list<Graph::vertex_descriptor> mConsumers;
  Graph::vertex_descriptor mMainConsumer;
};