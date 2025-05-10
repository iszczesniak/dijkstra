#ifndef DIJKSTRA_HPP
#define DIJKSTRA_HPP

#include "graph_interface.hpp"

#include <cassert>
#include <concepts>
#include <list>
#include <iterator>
#include <optional>

// The empty callable could be as simple as:

// template <typename ... Args>
// using NoCallable = decltype([](Args && ...){});

// The above crashes GCC 12.2.0, 9.3.0 and 10.1.0 with an internal
// error.  This works instead:

template <typename ... Args>
struct NoCallable
{
  void
  operator()(Args && ...) const
  {
  }
};

/**
 * Run the generic Dijkstra algorithm.
 */
template <typename Label, typename Permanent, typename Tentative,
          std::invocable<const Label &> V = NoCallable<const Label &>>
void
dijkstra(const Label &initial, Permanent &P, Tentative &T,
         const std::invocable<const Label &,
                              const Edge<Label> &> auto &f,
         const V &visit = {})
{
  // Boot the search.
  T.push(initial);

  while(!T.empty())
    {
      const auto &l = move_label(T, P);

      // Call the visit visitor.
      visit(l);

      // The target of the label.
      const auto &v = get_target(l);

      // Itereate over the out edges of vertex v.
      for(const auto &e: get_edges(v))
        relax(l, e, P, T, f);
    }
}

/**
 * Run the generic Dijkstra algorithm, stop at dst.
 */
template <typename Label, typename Permanent, typename Tentative,
          typename Vertex>
void
dijkstra(const Label &initial, Permanent &P, Tentative &T,
         const std::invocable<const Label &,
                              const Edge<Label> &> auto &f,
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
template <typename Label, typename Permanent, typename Tentative>
void
relax(const Label &l, const Edge<Label> &e,
      Permanent &P, Tentative &T,
      const std::invocable<const Label &,
                           const Edge<Label> &> auto &f)
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
      auto cls = f(l, e);

      for (auto &cl: cls)
	if (!has_better_or_equal(P, cl) && !has_better_or_equal(T, cl))
          T.push(std::move(cl));
    } catch (bool no_label)
    {
      assert(no_label);
    }
}

/**
 * Trace back the labels.
 */
template <typename Range, typename Permanent, typename Label,
          typename Functor>
auto
trace(const Permanent &P, const Functor &f,
      const Label &last, const Label &init)
{
  // The list of labels.
  std::list<Label> result;

  for(const auto &l: Range(P, f, last, init))
    result.push_front(l);

  return result;
}

/**
 * Trace back the labels.
 */
template <typename Range, typename Permanent, typename Vertex,
	  typename Label, typename Functor>
std::optional<std::list<Label>>
trace(const Permanent &P, const Functor &f,
      const Vertex &dst, const Label &init)
{
  // Make sure there is the solution for vertex dst.
  if (const auto &vd = P[get_key(dst)]; !std::empty(vd))
    {
      const auto &last = vd.front();
      // Move the result to the optional object we return.
      return trace<Range>(P, f, last, init);
    }

  // We return an empty optional, becase no path was found.
  return {};
}


#endif // DIJKSTRA_HPP
