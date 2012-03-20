/**
 * \file   HttpRequest.h
 * \author Pierre Rannou <pierre.rannou@epitech.eu>
 * \date   Mon Mar 12 07:38:31 2012
 *
 * \brief  HttpRequest declaration.
 *
 *
 */

#ifndef BREF_API_HTTPREQUEST_H
#define BREF_API_HTTPREQUEST_H

#include <deque>

#include "HttpConstants.h"
#include "HttpHeader.h"
#include "Version.h"

namespace bref {

/**
 * \brief HTTP request container
 *
 * Contains a HTTP request using HttpHeader
 *
 * \sa HttpResponse
 */
class BREF_DLL HttpRequest : public HttpHeader
{
private:
  /**
   * \brief Contains HTTP method
   * \sa RequestMethods::Type
   */
  request_methods::Type method_;

  /**
   * \brief Contains HTTP request URI
   */
  std::string          uri_;

  /**
   * \brief Contains used HTTP version
   */
  Version              version_;

public:
  /**
   * \brief Construct an empty request.
   *
   */
  HttpRequest();

  /**
   * \brief Destructor.
   *
   */
  ~HttpRequest();

  /**
   * \brief Get HTTP method
   *
   * \return Used HTTP method
   */
  request_methods::Type getMethod() const;

  /**
   * \brief Get HTTP request URI
   *
   * \return HTTP request URI
   */
  const std::string & getUri() const;

  /**
   * \brief Get current HTTP version
   *
   * \return Used HTTP version
   */
  const Version & getVersion() const;

  /**
   * \brief Set used HTTP method
   */
  void setMethod(request_methods::Type);

  /**
   * \brief Set asked HTTP URI
   */
  void setUri(const std::string &);

  /**
   * \brief Set HTTP version
   */
  void setVersion(const Version &);
};

}

#endif  // ! BREF_API_HTTPREQUEST_H
