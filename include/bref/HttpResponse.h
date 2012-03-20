/**
 * \file   HttpResponse.h
 * \author Pierre Rannou <pierre.rannou@epitech.eu>
 * \date   Mon Mar 12 07:48:34 2012
 *
 * \brief  HttpResponse declaration.
 *
 */

#ifndef BREF_API_HTTPRESPONSE_H_
#define BREF_API_HTTPRESPONSE_H_

#include "HttpConstants.h"
#include "HttpHeader.h"
#include "Version.h"
#include "Buffer.h"
#include "detail/BrefDLL.h"

#include <deque>

namespace bref {

/**
 * \brief HTTP response container
 *
 * Contains a HTTP request using HttpHeader
 * \sa HttpRequest
 */
class BREF_DLL HttpResponse : public HttpHeader
{
private:
  /**
   * \brief Contains used HTTP version
   */
  Version            version_;

  /**
   * \brief Contains HTTP result code
   *        For example : bref::status_codes::OK
   */
  status_codes::Type statusCode_;

  /**
   * \brief Contains HTTP result reason
   *        For example : "I'm a Teapot"
   */
  std::string        reason_;

public:
  /**
   * \brief Construct an empty HTTP response.
   */
  HttpResponse();

  /**
   * \brief Destructor.
   */
  ~HttpResponse();

  /**
   * \brief Get the current HTTP version
   *
   * \return Used HTTP version
   *
   * \sa setVersion()
   */
  const Version & getVersion() const;

  /**
   * \brief Get the HTTP result code
   *
   * \return HTTP result code
   *
   * \sa setStatus()
   */
  status_codes::Type getStatus() const;

  /**
   * \brief Get the HTTP result reason
   *
   * \return HTTP result reason
   *
   * \sa setReason()
   */
  const std::string & getReason() const;

  /**
   * \brief Get a buffer containing the response header as raw data.
   *        Example :
   *
   * \code  "HTTP/1.1 200 OK\r\n" \endcode
   *
   * \return The response header as raw data.
   */
  Buffer getRawData() const;

  /**
   * \brief Set HTTP version
   *
   * \param version The HTTP version.
   *
   * \sa getVersion()
   */
  void setVersion(const Version & version);

  /**
   * \brief Set HTTP result code
   *
   * \param type The HTTP result code.
   *
   * \sa getStatus()
   */
  void setStatus(status_codes::Type type);

  /**
   * \brief Set HTTP result reason
   *
   * \param reason The HTTP result reason.
   *
   * \sa getReason()
   */
  void setReason(const std::string & reason);
};

} // ! bref

#endif /* !BREF_API_HTTPRESPONSE_H_ */
