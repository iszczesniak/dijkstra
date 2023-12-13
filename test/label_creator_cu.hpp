#ifndef LABEL_CREATOR_CU_HPP
#define LABEL_CREATOR_CU_HPP

#include "generic_label_creator.hpp"

#include <list>

struct label_creator_cu: generic_label_creator
{
  using base_type = generic_label_creator;

  template <typename Label, typename Edge>
  auto
  operator()(const Label &l, const Edge &e) const
  {
    // Candidate cost and resources.
    auto [c_c, c_su] = base_type::operator()(l, e);

    std::list<Label> ls;

    for (auto &c_cu: c_su)
      // The candidate label.
      ls.emplace_back(e, c_c, std::move(c_cu));

    return ls;
  }
};

#endif // LABEL_CREATOR_CU_HPP
