#include "dijkstra.hpp"
// #include "standard_label_creator.hpp"
#include "standard_permanent.hpp"
#include "standard_tentative.hpp"
// #include "standard_tracer.hpp"
#include "graph.hpp"
#include "label_robe.hpp"
#include "units.hpp"

#include <iostream>
#include <string>

template <typename Edge>
using vertex_type_tmp = vertex<Edge, index<unsigned>,
                               name<std::string>>;

using edge_type = edge<vertex_type_tmp, weight<unsigned>>;
using vertex_type = vertex_type_tmp<edge_type>;
using graph_type = graph<vertex_type>;

int
main()
{
  graph_type g(2);
  auto &v1 = add_vertex(g, "v0");
  auto &v2 = add_vertex(g, "v1");
  add_edge(v1, v2, 1);

  // The null edge that is not part of the graph.
  edge_type null_edge(v1, v1, 0);
  // The label type, and the initial label.
  using robe_type = label_robe<edge_type, weight<int>>;
  robe_type initial(null_edge, 0);

  standard_permanent<robe_type> P(num_vertexes(g));
  standard_tentative<robe_type> T(num_vertexes(g));
  // dijkstra(initial, P, T, standard_label_creator<robe_type>());

  // // Get and return the path.
  // auto op = trace(initial, v2, generic_tracer(P));

  // if (op)
  //   {
  //     const auto &p = op.value();
  //     for(const auto &l: p)
  //       std::cout << l << std::endl;
  //   }
}
