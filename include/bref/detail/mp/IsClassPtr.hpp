/**
 * \file   IsClassPtr.hpp
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Mon Mar  5 22:57:08 2012
 *
 * \brief  IsClassPtr definition for SFINAE.
 *
 *
 */

#ifndef BREF_API_DETAIL_MP_ISCLASSPTR_HPP
#define BREF_API_DETAIL_MP_ISCLASSPTR_HPP

namespace bref {

/**
 * \namespace mp
 *
 * \brief Contain SFINAE (Substitution Failure Is Not An Error) definitions.
 *
 * An exemple of the usage:
\code
using namespace bref;

template <typename T>
typename mp::enable_if<mp::IsClassPtr<T>::value, int>::type isClassPtr()
{
  return 42;
}

template <typename T>
typename mp::disable_if<mp::IsClassPtr<T>::value, int>::type isClassPtr()
{
  return 1;
}

struct Toto
{ };

int	main()
{
  return (isClassPtr<Toto>() +   // 1 +
          isClassPtr<Toto *>()); // 42 -> 43
}
\endcode
 *
 */
namespace mp {

typedef char                  True;  // sizeof(True) == 1
typedef struct { True a[2]; } False; // sizeof(False) > 1

/**
 * enable_if and disable_if definitions.
 */
template <bool Cond, class T = void>
struct enable_if { };

template <class T>
struct enable_if<true, T>
{
  typedef T type;
};

template <bool Cond, class T = void> struct
disable_if { };

template <class T>
struct disable_if<false, T>
{
  typedef T type;
};

/**
 * IsClassPtr definition.
 */
// value is false for non pointer to class types
template <typename T>
struct IsClassPtr
{
  enum { value = false };
};

// value is true here if T is a pointer to class type
template <typename T>
struct IsClassPtr<T *>          // this is a pointer
{
  // it can have pointer-to-member
  template <typename C> static True  isClassPtr(int C::*);
  template <typename C> static False isClassPtr(...);
  enum { value = sizeof(isClassPtr<T>(0)) == sizeof(True) };
};

} // ! mp
} // ! bref

#endif /* !BREF_API_DETAIL_MP_ISCLASSPTR_HPP */
