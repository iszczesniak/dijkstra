#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include <boost/range.hpp>

#include <list>
#include <iterator>
#include <optional>

template <typename F, typename ... Args>
concept Callable = requires(F f, Args ... args)
{
 f(args...);
};

template <typename ... Args>
using EmptyCallable = decltype([](Args ...){});

/**
 * In graph g, find the shortest path to t.
 */
template <typename Graph, typename Label,
          typename Permanent, typename Tentative,
          Callable<const Label &> V = EmptyCallable<const Label &>>
void
dijkstra(const Graph &g, const Label &sl, Permanent &P, Tentative &T,
         const Callable<const Edge<Graph> &, const Label &> &f,
         const V &visit = {})
{
  // Boot the search.
  T.push(sl);

  while(!T.empty())
    {
      const auto &l = move_label(T, P);

      // Call the visit visitor.
      visit(l);

      // The target of the label.
      const auto &v = get_target(l);

      // Itereate over the out edges of vertex v.
      for(const auto &e: boost::make_iterator_range(out_edges(v, g)))
        relax(g, P, T, e, l, f);
    }
}

/**
 * Try to relax edge e, given label l.
 */
template <typename Graph, typename Label,
          typename Permanent, typename Tentative>
void
relax(const Graph &g, const Edge<Graph> &e, const Label &l,
      Permanent &P, Tentative &T, 
      const Callable<const Edge<Graph> &, const Label &> &f)
{
  try
    {
      // Candidate labels.
      auto cls = f(e, l);

      for (auto &cl: cls)
	if (!has_better_or_equal(P, cl) && !has_better_or_equal(T, cl))
	  {
	    purge_worse(T, cl);
	    // We push the new label after purging, so that purging
	    // has less work, i.e., a smaller Q.  Furthermore, in
	    // purge_worse we are using the <= operator, which would
	    // remove the label we push below.
	    T.push(std::move(cl));
	  }
    } catch (bool no_label)
    {
      assert(no_label);
    }
}

/**
 * Build the path by tracing labels.
 */
template <typename Permanent, typename Vertex, typename Label,
          typename Tracer>
std::optional<typename Tracer::path_t>
trace(const Permanent &P, Vertex dst, const Label &sl, Tracer &t)
{
  // Make sure there is the solution for vertex dst.
  if (const auto &vd = P[dst]; !std::empty(vd))
    {
      // This is the path we're building.
      typename Tracer::path_t result;

      // Get the initial label, i.e. the label for the destination.
      for(auto i = t.init(result, vd); *i != sl; i = t.advance(P, i))
        t.push(result, i);

      // Move the result to the optional object we return.
      return std::move(result);
    }

  // We return an empty optional, becase no path was found.
  return std::optional<typename Tracer::path_t>();
}

#endif // DIJKSTRA_HPP
