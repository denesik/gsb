#pragma once
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

class ResourceNetwork
{
public:
  ResourceNetwork();

  using PropertyType = float;
  using EdgeWeightProperty = boost::property<boost::edge_weight_t, PropertyType>;

  using Traits = boost::adjacency_list_traits<boost::vecS, boost::vecS, boost::directedS>;
  using Graph = boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
    boost::property<boost::vertex_name_t, std::string,
      boost::property<boost::vertex_index_t, long,
        boost::property<boost::vertex_color_t, boost::default_color_type,
          boost::property<boost::vertex_distance_t, long,
            boost::property<boost::vertex_predecessor_t, Traits::edge_descriptor>
          >
        >
      >
    >,

    boost::property<boost::edge_capacity_t, PropertyType,
      boost::property<boost::edge_residual_capacity_t, PropertyType,
        boost::property<boost::edge_reverse_t, Traits::edge_descriptor>
      >
    >
  >;

  Graph g;

  Graph::vertex_descriptor AddProducer();
  Graph::vertex_descriptor AddConsumer();
  void AddTransmitter(Graph::vertex_descriptor from, Graph::vertex_descriptor to, PropertyType weight);
  PropertyType MaxFlow();

  void Reset();

  std::list<Graph::vertex_descriptor> mProducers;
  Graph::vertex_descriptor mMainProducer;

  std::list<Graph::vertex_descriptor> mConsumers;
  Graph::vertex_descriptor mMainConsumer;
};