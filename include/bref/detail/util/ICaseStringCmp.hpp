/**
 * \file   ICaseStringCmp.hpp
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Thu Apr 12 02:12:52 2012
 *
 * \brief  ICaseStringCmp class definition.
 *
 */

#ifndef BREF_DETAIL_UTIL_ICASESTRINGCMP_HPP_
#define BREF_DETAIL_UTIL_ICASESTRINGCMP_HPP_

#pragma once

#include <string>
#include <functional>
#include <cctype>

namespace bref {
namespace util {

/**
 * \brief Case insensitive char comparision function.
 */
inline bool icaseCharCmp(char a, char b)
{
  return std::tolower(a) < std::tolower(b);
}

/**
 * \brief Case insensitive string comparison.
 */
struct ICaseStringCmp : public std::binary_function<std::string, std::string, bool>
{
  bool operator()(const std::string & a, const std::string & b) const
  {
    return std::lexicographical_compare(a.begin(), a.end(),
                                        b.begin(), b.end(),
                                        &icaseCharCmp);
  }
};

} // ! util
} // ! bref

#endif /* !BREF_DETAIL_UTIL_ICASESTRINGCMP_HPP_ */
