#ifndef LABEL_ROBE_HPP
#define LABEL_ROBE_HPP

#include "graph_interface.hpp"

#include <iostream>
#include <utility>

// Important: this class does not offer comparison operatos, because
// the operators of the base type are used.
template <typename Edge, typename Label>
struct label_robe: Label
{
  using label_type = Label;
  const Edge &m_edge;

  template <typename ... Args>
  label_robe(const Edge &edge, Args && ... args):
    label_type(std::forward<Args>(args) ...), m_edge(edge)
  {
  }
};

template <typename Edge, typename Label>
std::ostream &
operator << (std::ostream &os, const label_robe<Edge, Label> &r)
{
  os << "label_robe("
     << "label = " << static_cast<const Label &>(r) << ", "
     << "edge = " << r.m_edge << ")";
                                 
  return os;                    
}

template <typename Label, typename Edge>
const auto &
get_edge(const label_robe<Label, Edge> &r)
{
  return r.m_edge;
}

template <typename Label, typename Edge>
const auto &
get_target(const label_robe<Label, Edge> &r)
{
  return get_target(r.m_edge);
}

template <typename Label, typename Edge>
const auto &
get_key(const label_robe<Label, Edge> &r)
{
  return get_key(get_target(r.m_edge));
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
