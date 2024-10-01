#include "dijkstra.hpp"
#include "generic_label.hpp"
#include "generic_path_range.hpp"
#include "generic_permanent.hpp"
#include "generic_tentative.hpp"
#include "graph.hpp"
#include "label_creator_cu.hpp"
#include "label_robe.hpp"
#include "units.hpp"

#include <iostream>
#include <string>

template <typename Edge>
using vertex_type_tmp = vertex<Edge, key<unsigned>,
                               name<std::string>>;

using edge_type = edge<vertex_type_tmp, weight<unsigned>,
                       resources<SU>>;
using vertex_type = vertex_type_tmp<edge_type>;
using graph_type = graph<vertex_type>;

int
main()
{
  vertex_type v(0, std::string());

  graph_type g(2);
  auto &v1 = add_vertex(g, "v0");
  auto &v2 = add_vertex(g, "v1");
  add_edge(v1, v2, 1, SU{{0, 5}});

  // The null edge that is not part of the graph.
  edge_type null_edge(v1, v1, 0, SU{});
  // The label type, and the initial label.
  using robe_type = label_robe<edge_type, generic_label<int, CU>>;
  robe_type initial(null_edge, 0, CU(0, 10));

  generic_permanent<robe_type> P(num_vertexes(g));
  generic_tentative<robe_type> T(num_vertexes(g));
  label_creator_cu f;
  dijkstra(initial, P, T, f);

  // The label at the destination.
  const auto &l = P[get_key(v2)].front();

  for(const auto &l: generic_path_range(P, f, l, initial))
    std::cout << l << std::endl;
}
