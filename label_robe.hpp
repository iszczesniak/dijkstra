#ifndef LABEL_ROBE_HPP
#define LABEL_ROBE_HPP

#include "graph_interface.hpp"

#include <iostream>
#include <utility>

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

  // We delegate == and <=> to label_type.  We do not want the default
  // implementations, because m_edge should not take part.
  constexpr bool operator == (const label_robe &l) const
  {
    return static_cast<const label_type &>(*this)
      == static_cast<const label_type &>(l);
  }

  constexpr auto operator <=> (const label_robe &l) const
  {
    return static_cast<const label_type &>(*this)
      <=> static_cast<const label_type &>(l);
  }
};

template <typename Edge, typename Label>
std::ostream &
operator << (std::ostream &os, const label_robe<Edge, Label> &r)
{
  os << "label_robe("
     << "edge = " << r.m_edge << ", "
     << "label = " << static_cast<const Label &>(r)
     << ")";
                                 
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
