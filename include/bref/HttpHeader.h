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

namespace bref {

/**
 * \brief A map seems to be sufficient to represent HTTP Header fields.
 * \sa BrefValue
 */
typedef std::map<std::string, BrefValue> HttpHeader;

} // ! bref

#endif /* !BREF_API_HTTPHEADER_H_ */
