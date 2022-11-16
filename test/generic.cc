#include "dijkstra.hpp"
#include "generic_label.hpp"
#include "generic_label_creator.hpp"
#include "generic_permanent.hpp"
#include "generic_tentative.hpp"
#include "generic_tracer.hpp"
#include "graph.hpp"
#include "label_robe.hpp"
#include "units.hpp"

#include <iostream>

int
main()
{
  using edge_type = edge<unsigned, SU>;
  graph<vertex<edge_type>> g(2);
  auto &v1 = add_vertex(g, "v0");
  auto &v2 = add_vertex(g, "v1");
  add_edge(v1, v2, 1, {{0, 5}});

  // The null edge that is not part of the graph.
  edge_type null_edge(v1, v1, 0, {});
  // The label type, and the initial label.
  using robe_type = label_robe<edge_type, generic_label<int, CU>>;
  robe_type initial(null_edge, 0, CU(0, 10));

  generic_permanent<robe_type> P(num_vertexes(g));
  generic_tentative<robe_type> T(num_vertexes(g));
  dijkstra(initial, P, T, generic_label_creator<robe_type>());

  // Get and return the path.
  auto op = trace(initial, v1, generic_tracer(P));

  if (op)
    {
      const auto &p = op.value();
      for(const auto &l: p)
        std::cout << l;
    }
}
