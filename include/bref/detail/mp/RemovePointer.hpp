/**
 * \file   RemovePointer.hpp
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Sun Mar 11 05:51:26 2012
 *
 * \brief  RemovePointer definition.
 *
 */

#ifndef BREF_API_DETAIL_MP_REMOVEPOINTER_HPP
#define BREF_API_DETAIL_MP_REMOVEPOINTER_HPP

namespace bref {
namespace mp {

/**
 * \brief Type trait that remove a pointer of a given type if any.
 */
template <typename T> struct RemovePointer {
  typedef T type;
};
template <typename T> struct RemovePointer<T *>
{
  typedef T type;
};

} // ! mp
} // ! bref

#endif /* !BREF_API_DETAIL_MP_REMOVEPOINTER_HPP */
