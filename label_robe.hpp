#ifndef LABEL_ROBE_HPP
#define LABEL_ROBE_HPP

template <typename Edge, typename Label>
struct label_robe: Label
{
  using base = Label;
  Edge &m_edge;

  label_robe(Edge &edge): m_edge(edge), base()
  {
  }
};

#endif // LABEL_ROBE_HPP
