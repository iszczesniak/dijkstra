#include "dijkstra.hpp"
#include "generic_label.hpp"
#include "graph.hpp"
#include "units.hpp"

int
main()
{
  graph g(2);
  auto &v1 = add_vertex(g, "v0");
  auto &v2 = add_vertex(g, "v1");
  add_edge(v1, v2, 1, {0, 5});

  using label_type = generic_label<int, SU>;
  label_type initial(0, {0, 10});
  //  generic_permament<label_type> P(num_vertexes(g));
  //generic_tentative<label_type> T(num_vertexes(g));
  //dijkstra(label_robe(initial, null_edge), P, T);
}
