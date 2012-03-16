/**
 * \file   Buffer.h
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Wed Mar 14 00:38:02 2012
 *
 * \brief  Buffer definition.
 *
 */

#ifndef BREF_API_BUFFER_H_
#define BREF_API_BUFFER_H_

#include <vector>

namespace bref {

/**
 * \brief Type used for buffering operation.
 *
 * A vector seems to be the best fit for a buffer. It's efficient,
 * well suited for read/write operation because it can act as a C
 * container.
 */
typedef std::vector<char> Buffer;

} // ! bref

#endif /* !BREF_API_BUFFER_H_ */
