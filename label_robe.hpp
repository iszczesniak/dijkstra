#ifndef LABEL_ROBE_HPP
#define LABEL_ROBE_HPP

template <typename Label, typename Edge>
struct label_robe: Label
{
  using base = Label;
  Edge &m_edge;

  label_robe(Edge &edge): m_edge(edge), base()
  {
  }
};

template <typename Label, typename Edge>
const auto &
get_target(const label_robe<Label, Edge> &r)
{
  return get_target(r.m_edge);
}

#endif // LABEL_ROBE_HPP
