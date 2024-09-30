#include "dijkstra.hpp"
#include "standard_label_creator.hpp"
#include "standard_constrained_label_creator.hpp"
#include "standard_permanent.hpp"
#include "standard_tentative.hpp"
#include "standard_path_range.hpp"
#include "graph.hpp"
#include "label_robe.hpp"
#include "props.hpp"
#include "units.hpp"

#include <iostream>
#include <string>

template <typename Edge>
using vertex_type_tmp = vertex<Edge, key<unsigned>,
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
  add_edge(v1, v2, 10);

  // The null edge that is not part of the graph.
  edge_type null_edge(v1, v1, 0);
  // The label type (integer weight only), and the initial label.
  using robe_type = label_robe<edge_type, weight<unsigned>>;
  robe_type initial(null_edge, 0);

  // There is no path of weight 5 or less.
  {
    standard_permanent<robe_type> P(num_vertexes(g));
    standard_tentative<robe_type> T(num_vertexes(g));
    dijkstra(initial, P, T, standard_constrained_label_creator(5u));
    // There is no label for vertex v2.
    assert(!P[get_key(v2)]);
  }

  {
    standard_permanent<robe_type> P(num_vertexes(g));
    standard_tentative<robe_type> T(num_vertexes(g));
    dijkstra(initial, P, T, standard_label_creator());
    assert(P[get_key(v2)]);

    // The label at the destination.
    const auto &l = P[get_key(v2)].value();

    for(const auto &l: standard_path_range(P, l, initial))
      std::cout << l << std::endl;
  }
}
