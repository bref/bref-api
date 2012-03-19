/**
 * \file   IConfHelper.h
 * \author Vincent Giersch <vincent.giersch@epitech.eu>
 * \date   Mon Mar 12 07:51:34 2012
 *
 * \brief  IConfHelper interface declaration.
 *
 */

#ifndef BREF_API_ICONFHELPER_H_
#define BREF_API_ICONFHELPER_H_
#pragma once

#include <string>

#include "detail/util/NonCopyable.hpp"
#include "BrefValue.h"
#include "HttpRequest.h"

namespace bref {

/**
 * \brief Helper class for BrefValue and the server configuration.
 *
 * This class aimed to give an easier access to the server
 * configuration.
 *
 */
class IConfHelper : public util::NonCopyable
{
public:
  virtual ~IConfHelper() { }

  /**
   * \brief  Return an acceptable value for "key" param.
   *
   * \return Return value if exists, a BrefValue with a "nullType"
   *         otherwise.
   */
  virtual const BrefValue & findValue(std::string const & key) const = 0;

  /**
   * \brief Return an acceptable value for "key" param, using request.
   *
   * This can be use, for example to find virtual host configuration
   * value for a given key.
   *
   * \return Return value if exists, a BrefValue with a "nullType"
   *         otherwise.
   */
  virtual const BrefValue & findValue(std::string const & key,
                                      HttpRequest const & request) const = 0;
};

} // ! bref

#endif /* !BREF_API_ICONFHELPER_H_ */
