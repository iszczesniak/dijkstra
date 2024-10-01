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

  // We do want the default implementation of == for the robed label,
  // because the path iterators need it.  The iterators use == to stop
  // when the initial robed label is reached.  The problem is that
  // comparing the naked labels (i.e., label_type) is not enough,
  // because they all can be equal along the path (e.g, 0), so we only
  // know when to stop when the initial robed label is reached.  We
  // recognize the initial robed label by its edge.  And so edges do
  // have to be compared.  The default implementation first compares
  // the based objects, then the member objects.

  // Yet, the defautl implementation does not compile, and the
  // compiler (both clang and GCC) complains that they don't like the
  // reference member m_edge.  I don't know what's going on.  So I
  // have to implement the default behavious myself.
  constexpr bool operator == (const label_robe &l) const
  {
    return static_cast<const label_type &>(*this)
      == static_cast<const label_type &>(l) &&
      m_edge == l.m_edge;
  }

  // This comparison operator is used to establish order between robed
  // labels.  The order is the same as for the naked labels, so we
  // relagate.  We do not want the edge to take part in comparison.
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
