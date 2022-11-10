#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "label_robe.hpp"

#include <cassert>
#include <concepts>
#include <list>
#include <iterator>
#include <optional>

// Empty callable could be as simple as:

// template <typename ... Args>
// using EmptyCallable = decltype([](Args ...){});

// The above crashes GCC 9.3.0 and 10.1.0 with an internal error.
// This works:

template <typename ... Args>
struct NoCallable
{
  void
  operator()(Args ...) const
  {
  }
};

/**
 * Run the generic Dijkstra algorithm.
 */
template <typename Label, typename Edge,
          typename Permanent, typename Tentative,
          std::invocable<const Label &> V = NoCallable<const Label &>>
void
dijkstra(const label_robe<Label, Edge> &initial,
         Permanent &P, Tentative &T,
         const std::invocable<const Label &, const Edge &> auto &f,
         const V &visit = {})
{
  // Boot the search.
  T.push(initial);

  while(!T.empty())
    {
      const auto &l = move_label(T, P);

      // Call the visit visitor.
      visit(l);

      // The target of the label, the label robe, specifically.
      const auto &v = get_target(l);

      // Itereate over the out edges of vertex v.
      for(const auto &e: get_edges(v))
        relax(l, e, P, T, f);
    }
}

/**
 * Run the generic Dijkstra algorithm, stop at dst.
 */
template <typename Label, typename Edge, typename Vertex,
          typename Permanent, typename Tentative>
void
dijkstra(const label_robe<Label, Edge> &initial,
         Permanent &P, Tentative &T,
         const std::invocable<const Edge &, const Label &> auto &f,
         const Vertex &dst)
{
  // Run the search.
  try
    {
      auto visit = [dst](const auto &l)
                   {
                     if (dst == get_target(l))
                       throw true;
                   };

      // Run the search.
      dijkstra(initial, P, T, f, visit);
    }
  catch (bool status)
    {
      assert(status);
    }
}

// Move the best label from T (the tentative labels) to P (the
// permanent labels), and return a reference to the label in the new
// place.
template <typename TC, typename PC>
const auto &
move_label(TC &T, PC &P)
{
  return P.push(T.pop());
}

/**
 * Try to relax edge e, given label l.
 */
template <typename Graph, typename Label,
          typename Permanent, typename Tentative>
void
relax(const Graph &g, const Edge<Graph> &e, const Label &l,
      Permanent &P, Tentative &T, 
      const std::invocable<const Edge<Graph> &, const Label &>
      auto &f)
{
  // This try block gives me a headache.  I need this, because for the
  // standard_dijkstra there might not be candidate labels returned by
  // functor f, and I signal this by throwing a bool.  However, I
  // would prefer to return an empty std::optional, but the problem is
  // that it cannot be used in the foreach loop, since it has no
  // begin, and end functions.  I leave this as future work.
  try
    {
      // Candidate labels.
      auto cls = f(e, l);

      for (auto &cl: cls)
	if (!has_better_or_equal(P, cl) && !has_better_or_equal(T, cl))
	  {
	    purge_worse(T, cl);
	    // We push the new label after purging, so that purging
	    // has less work, i.e., a smaller T.  Furthermore, in
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
std::optional<typename Tracer::path_type>
trace(const Permanent &P, Vertex dst, const Label &sl, Tracer &t)
{
  // Make sure there is the solution for vertex dst.
  if (const auto &vd = P[index(dst)]; !std::empty(vd))
    {
      // This is the path we're building.
      typename Tracer::path_type result;

      // Get the initial label, i.e. the label for the destination.
      for(auto i = t.init(result, vd); *i != sl; i = t.advance(P, i))
        t.push(result, i);

      // Move the result to the optional object we return.
      return std::move(result);
    }

  // We return an empty optional, becase no path was found.
  return std::optional<typename Tracer::path_type>();
}

#endif // DIJKSTRA_HPP
