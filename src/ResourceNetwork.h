#pragma once

#include <boost/graph/adjacency_list.hpp>

class ResourceNetwork
{
public:

  template <class Graph> 
  struct exercise_vertex {
    using Vertex = typename graph_traits<Graph>::vertex_descriptor;

    void operator()(const Vertex& v) const
    {
    }
  };

  using Graph = boost::adjacency_list<boost::listS, boost::vecS, boost::bidirectionalS>;
  typedef std::pair<int, int> Edge;

  void Add();
};