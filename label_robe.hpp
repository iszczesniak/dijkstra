#ifndef LABEL_ROBE_HPP
#define LABEL_ROBE_HPP

#include "graph_interface.hpp"

#include <utility>

template <typename Edge, typename Label>
struct label_robe: Label
{
  using base_type = Label;
  const Edge &m_edge;

  template <typename ... Args>
  label_robe(const Edge &edge, Args && ... args):
    base_type(std::forward<Args>(args) ...), m_edge(edge)
  {
  }
};

template <typename Label, typename Edge>
const auto &
get_target(const label_robe<Label, Edge> &r)
{
  return get_target(r.m_edge);
}

template <typename Edge, typename Label>
struct edge_traits<label_robe<Edge, Label>>
{
  using type = Edge;
};

template <typename Edge, typename Label>
struct weight_traits<label_robe<Edge, Label>>
{
  using type = Weight<Label>;
};

#endif // LABEL_ROBE_HPP
