#include "ResourceNetwork.h"
#include <limits>
#include <boost/graph/boykov_kolmogorov_max_flow.hpp>
#include <boost/numeric/ublas/matrix.hpp>

ResourceNetwork::ResourceNetwork()
{
  Reset();
}

constexpr auto maxPair = std::make_pair(ResourceNetwork::ProrertySubtype(), std::numeric_limits<ResourceNetwork::ProrertySubtype>::max());

ResourceNetwork::V ResourceNetwork::AddProducer()
{
  V v0 = boost::add_vertex(g);
  mProducers.push_back(v0);
  boost::add_edge(mMainProducer, v0, maxPair, g);
  return v0;
}

ResourceNetwork::V ResourceNetwork::AddConsumer()
{
  V v0 = boost::add_vertex(g);
  mConsumers.push_back(v0);
  boost::add_edge(v0, mMainConsumer, maxPair, g);
  return v0;
}

void ResourceNetwork::AddTransmitter(V from, V to, ProrertySubtype weight)
{
  auto res = boost::add_edge(from, to, std::make_pair(0.f, weight), g);
}

inline ResourceNetwork::PropertyType top(void *p)
{
  return * reinterpret_cast<ResourceNetwork::PropertyType *>(p);
}

ResourceNetwork::ProrertySubtype ResourceNetwork::MaxFlow()
{
  V u = mMainProducer;
  std::deque<V> toVisit;

  std::vector<bool>            mark(boost::distance(boost::vertices(g)));
  std::vector<ProrertySubtype> dist(boost::distance(boost::vertices(g)));
  std::vector<ProrertySubtype> push(boost::distance(boost::vertices(g)));
  std::vector<V>               pred(boost::distance(boost::vertices(g)));

  toVisit.push_back(mMainProducer);

  while (!toVisit.empty())
  {
    auto v = *toVisit.begin();
    toVisit.pop_front();
    mark[v] = true;
    pred[v] = u;

    auto edges = boost::out_edges(v, g);
    for (auto edge = edges.first; edge != edges.second; ++edge)
    {
      if (edge->m_target == mMainConsumer)
        break;

      auto eprop = top(edge->get_property());
      push[v] = std::min(push[u], eprop.second - eprop.first);

      if(!mark[edge->m_target])
        toVisit.push_back(edge->m_target);
    }

    u = v;
  }

  return 0;
}

void ResourceNetwork::Reset()
{
  g.clear();

  mMainProducer = boost::add_vertex(g);
  mMainConsumer = boost::add_vertex(g);
}
