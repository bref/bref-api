/**
 * \file   Function.hpp
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Sat Mar  3 17:39:26 2012
 *
 * \brief  Partial C++ std::function implementation.
 *
 * \see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1402.html
 * \see http://www.codeproject.com/Articles/7150/Member-Function-Pointers-and-the-Fastest-Possible
 *      and the /vmg switch and forward declared class pointer size.
 *
 * \note The Function implementation is missing the optionnal
 *       allocator template type.
 */

#ifndef BREF_API_DETAIL_FUNCTION_HPP_
#define BREF_API_DETAIL_FUNCTION_HPP_

#include "FunctionFwd.hpp"
#include "mp/IsClassPtr.hpp"
#include "mp/RemovePointer.hpp"

#include <stdexcept>

namespace bref {

/**
 * \brief Wrapper class to reference a variable.
 *
 * A copy-constructible and assignable wrapper around an object of
 * type \c T&.
 *
 * \tparam T
 *      The type referenced.
 */
template<typename T>
class ReferenceWrapper
{
public:
  typedef T type;

  explicit ReferenceWrapper(T & t) : data(t)
  { }

  operator T & () const { return data; }
  T & get() const       { return data; }

private:
  T & data;                     // exposition only
};

/**
 * \return ReferenceWrapper<T>(t)
 */
template<typename T> ReferenceWrapper<T> ref(T & t)
{
  return ReferenceWrapper<T>(t);
}

/**
 *  Requires: T cannot be a function type.
 *  [This requirement may be unnecessary depending on the resolution of core language issue 295]
 *
 *  \return ReferenceWrapper<T const>(t)
 *  Throws: will not throw.
 */
template<typename T> ReferenceWrapper<T const> cref(const T& t)
{
  return ReferenceWrapper<T const>(t);
}

/**
 * The BadFunctionCallException exception class is thrown primarily when a
 * polymorphic adaptor is invoked but is empty (see 20.3.10).
 */
class BadFunctionCallException : public std::runtime_error
{
public:
  // 20.3.9.1 constructor
  BadFunctionCallException()
    : std::runtime_error("call to empty Function object")
  { }
};

/* Function from 1 to 10 parameters generation in Shell script (Bash, ZSH):
   for N in {0..10}; do
       echo -n '#define FUNCTION_ARGUMENTS(a, b, c)'
       for i in $(seq 1 $N); do
           case "$i" in
               1) letter="a";;
               2) letter="b";;
               *) letter="c" ;;
           esac
           echo -n " $letter($i)";
       done;
       echo -e '\n#include "Function.def"'
   done;
*/
#define FUNCTION_ARGUMENTS(a, b, c)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1) b(2)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1) b(2) c(3)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1) b(2) c(3) c(4)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1) b(2) c(3) c(4) c(5)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1) b(2) c(3) c(4) c(5) c(6)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1) b(2) c(3) c(4) c(5) c(6) c(7)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1) b(2) c(3) c(4) c(5) c(6) c(7) c(8)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1) b(2) c(3) c(4) c(5) c(6) c(7) c(8) c(9)
#include "Function.def"
#define FUNCTION_ARGUMENTS(a, b, c) a(1) b(2) c(3) c(4) c(5) c(6) c(7) c(8) c(9) c(10)
#include "Function.def"

// 20.3.10.5 specialized algorithms
// template<typename Function1, typename Function2>
// void swap(function<Function1>&, function<Function2>&);

// // 20.3.10.6 undefined operators
// template<typename Function1, typename Function2>
// void operator==(const function<Function1>&, const function<Function2>&);
// template<typename Function1, typename Function2>
// void operator!=(const function<Function1>&, const function<Function2>&);

} // ! bref

#endif /* !BREF_API_DETAIL_FUNCTION_HPP_ */
