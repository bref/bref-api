/**
 * \file   Function.hpp
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Sun Mar 11 05:44:18 2012
 *
 * \brief  Include the Function template class definition.
 *
 */

#ifndef BREF_API_FUNCTION_HPP_
#define BREF_API_FUNCTION_HPP_

/**
 * \class bref::Function
 *
 *
 * \brief Polymorphic Function Object Wrapper.
 *
 * The Function class is a personal challenge to implement the following
 * proposal to add a Polymorphic Function Object Wrapper to the C++
 * Standard Library. It's based on the Boost.Function library and have
 * the added functionnality of bounded pointer-to-member function.
 * You're not asked to understand the implementation, just invited to
 * look into it's powerful features.
 *
 * The implementation try to limit the size of the Function to be as
 * small as possible, and with a small use of allocation (only used
 * when there was no other way to do).
 *
 * A function handle between 0 and 10 parameters, but this number can
 * easily be extented to a larger number.
 *
 * If you want to understand the Function class usage you should
 * probably look into:
 * - Boost.Function - http://www.boost.org/doc/libs/1_49_0/doc/html/function.html
 * - std::function
 * - http://www.codeproject.com/Articles/11015/The-Impossibly-Fast-C-Delegates
 *   where the idea of embedding a bounded pointer come from.
 *
 * An finally http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2002/n1402.html
 * which was what I tried to implement. With a difference in the names used:
 * - reference_wrapper -> ReferenceWrapper
 * - ref -> ref
 * - cref -> cref
 * - bad_function_call -> BadFunctionCallException
 *
 * \warning Calling a null function (not bounded to anything) will
 *          throw a bref::BadFunctionCallException.
 *
 *
 * A list of \e things Function can bind:
 * - a pointer to function
\code
int add(int a, int b)
{
  return a + b;
}

bref::Function<int (int, int)> addFunction(&add);
std::cout << addFunction(4, 12) << std::endl; // print 16
\endcode
 *
 * - a function object aka Functor
\code
struct Adder
{
  int initValue;
  Adder(int val) : initValue(val)
  { }

  int operator()(int p) {
    return initValue + p;
  }
};

bref::Function<int (int)> addFunction(Adder(4));
std::cout << addFunction(12) << std::endl; // print 16
\endcode
 *
 * - A pointer-to-member function
\code
struct Car
{
  int initValue;

  void vroom(const std::string & id) {
    std::cout << id << " vrooOOOo00ooommm !!!" << std::endl;
  }
};

bref::Function<void (Car *, const std::string &)> addFunction(&Car::vroom);
Car                                              c;

addFunction(&c, "Flash Mc Queen");
\endcode
 *
 * - A bounded pointer-to-member function
\code
  Car c;
  bref::Function<void (const std::string &)> f(&c, &Car::vroom);
  bref::Function<void (const std::string &)> f2;

  f2.bind(&c, &Car::vroom);
  f("Martin");
  f2("Martin");
\endcode
 *
 * You can check for emptiness with a boolean conversion operator or
 * the empty() method.
 *
 */

/**
 * \class bref::BadFunctionCallException
 * \brief Exception thrown by bref::Function.
 *
 * This exception is throw by a bref::Function when a null function
 * object is called by the \c operator()(Args...).
 *
 */
#include "detail/Function.hpp"

#endif /* !BREF_API_FUNCTION_HPP_ */
