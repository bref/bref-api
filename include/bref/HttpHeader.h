/**
 * \file   HttpHeader.h
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Mon Mar 12 07:40:03 2012
 *
 * \brief  HttpHeader declaration.
 *
 */

#ifndef BREF_API_HTTPHEADER_H_
#define BREF_API_HTTPHEADER_H_

#include <map>
#include <string>
#include "BrefValue.h"
#include "detail/util/ICaseStringCmp.hpp"

namespace bref {

/**
 * \brief A map seems to be sufficient to represent HTTP Header fields.
 *
 * \note RFC2616, section 4.2 says: "Field names are
 *       case-insensitive.". The third parameter util::ICaseStringCmp
 *       allows the map to consider the keys "KEY", "key", "Key", etc,
 *       to be equivalent.
 *
 * \sa BrefValue
 */
typedef std::map<std::string, BrefValue, util::ICaseStringCmp> HttpHeader;

} // ! bref

#endif /* !BREF_API_HTTPHEADER_H_ */
