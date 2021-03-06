#ifndef LATTICES_LATTICE_H_
#define LATTICES_LATTICE_H_

#include <type_traits>

namespace latticeflow {

// Consider a partially ordered set (S, <=). An *upper bound* z of two elements
// a and b in S is an element of S such that z >= a and y >= b. An upper bound
// is said to be a *least upper bound* if it is less than or equal to all other
// upper bounds. That is, z is the least upper bound of a and b if
//
//   1. z is an upper bound of a and b, and
//   2. z is less than or equal to all upper bounds of a and b.
//
// Note that by the antisymmetry of <=, least upper bounds are unique.
//
// A *join semilattice* (or upper semillatice) is a partially ordered set (S,
// <=) such that every pair of elements in S has a least upper bound. The least
// upper bound of two elements x and y is known as their join, which we will
// denote by join(x, y). Note that join is associative, commutative, and
// idempotent.
//
//   - associative: for all x, y, z. join(x, join(y, z)) == join(join(x, y), z)
//   - commutative: for all x, y. join(x, y) == join(y, x)
//   - idempotent:  for all x. join(x, x) == x
//
// Dually, any structure (S, join) of a set S and an associative, commutative,
// idempotent operator join induces a partial order on S: x <= y if and only if
// join(x, y) == y. The structure (S, <=) of the set and the induced partial
// order forms a semillatice.
//
// We represent semilattices using CRTP where an instance of type `Lattice<L,
// T>` represents an instance of a semillatice with type `T` implemented by a
// subclass `L`. Refer to this directory for many examples.
//
// Note that semilattices are not required to have a bottom element, but if
// they do, it is recommended that the default constructor of the implementing
// class initialize to the bottom element.
template <typename L, typename T>
class Lattice {
 public:
  // The type of the lattice. Being able to access this type comes in handy
  // when doing a bit of metaprogramming. See the static_asserts in
  // pair_lattice.h for an example.
  using lattice_type = T;

  // Returns an element of the semilattice.
  virtual const T& get() const = 0;

  // Joins another instance of the semilattice into this one.
  virtual void join(const L& other) = 0;
};

// Returns whether `l == r` according to the partial order of the lattice.
template <typename T>
typename std::enable_if<
    std::is_base_of<Lattice<T, typename T::lattice_type>, T>::value, bool>::type
operator==(const T& l, const T& r) {
  return l.get() == r.get();
}

// Returns whether `l != r` according to the partial order of the lattice.
template <typename T>
typename std::enable_if<
    std::is_base_of<Lattice<T, typename T::lattice_type>, T>::value, bool>::type
operator!=(const T& l, const T& r) {
  return l.get() != r.get();
}

// Returns true if `l <= r` according to the partial order of the lattice.
template <typename T>
typename std::enable_if<
    std::is_base_of<Lattice<T, typename T::lattice_type>, T>::value, bool>::type
operator<=(T l, const T& r) {
  l.join(r);
  return l.get() == r.get();
}

}  // namespace latticeflow

#endif  // LATTICES_LATTICE_H_
