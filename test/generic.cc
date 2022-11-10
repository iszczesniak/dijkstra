#include "dijkstra.hpp"
#include "generic_label.hpp"
#include "generic_permanent.hpp"
#include "generic_tentative.hpp"
#include "graph.hpp"
#include "units.hpp"

int
main()
{
  graph<vertex<edge<unsigned, SU>>> g(2);
  auto &v1 = add_vertex(g, "v0");
  auto &v2 = add_vertex(g, "v1");
  add_edge(v1, v2, 1, {{0, 5}});

  using label_type = generic_label<int, SU>;
  label_type initial(0, {{0, 10}});

  // The null edge that is not part of the graph.
  edge<unsigned, SU> null_edge(v1, v1, 0, {});

  generic_permanent<label_type> P(num_vertexes(g));
  generic_tentative<label_type> T(num_vertexes(g));
  // dijkstra(label_robe(initial, null_edge), P, T);
}
