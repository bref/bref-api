/**
 * \file   FunctionFwd.hpp
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Sun Mar 11 05:55:29 2012
 *
 * \brief  Function and cie forward declarations.
 *
 *
 */

#ifndef BREF_API_FUNCTIONFWD_HPP_
#define BREF_API_FUNCTIONFWD_HPP_

namespace bref {

template<typename T> class ReferenceWrapper;

template<typename T> ReferenceWrapper<T> ref(T& t);
template<typename T> ReferenceWrapper<T const> cref(const T& t);

class BadFunctionCallException;

template<typename T, typename SfinaeHelper = void>
class Function;

template<typename F>
void swap(Function<F> &, Function<F> &);

// template<typename F1, typename F2>
// void operator==(const Function<F1> &, const Function<F2> &);

// template<typename F1, typename F2>
// void operator!=(const Function<F1> &, const Function<F2> &);

} // ! bref

#endif /* !BREF_API_FUNCTIONFWD_HPP_ */
