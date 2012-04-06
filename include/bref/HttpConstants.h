/**
 * \file   HttpConstants.h
 * \author Pierre Rannou <pierre.rannou@epitech.eu>
 * \date   Mon Mar 12 07:12:49 2012
 *
 * \brief  Http constants definitions.
 *
 *
 */

#ifndef BREF_API_HTTPCONSTANTS_H
#define BREF_API_HTTPCONSTANTS_H

namespace bref {

/**
 * \brief Namespace containing the various HTTP methods.
 * \sa request_methods::Type
 */
namespace request_methods {

  /**
   * \brief Enumeration containing the various HTTP methods.
   */
  enum Type {
    UndefinedRequestMethod = 0,
    Options,
    Get,
    Head,
    Post,
    Put,
    Delete,
    Trace,
    Connect
  };
} // ! request_methods

/**
 * \brief Namespace containing the various HTTP status codes.
 * \sa status_code::Type
 */
namespace status_codes {

/**
 * \brief Enumeration containing the various HTTP status codes.
 */
enum Type {
  // 0 : Default value
  UndefinedStatusCode          = 0,

  // 1xx: Informational
  Continue                     = 100,
  SwitchingProtocols           = 101,

  // 2xx: Success
  OK                           = 200,
  Created                      = 201,
  Accepted                     = 202,
  NonAuthoritativeInformation  = 203,
  NoContent                    = 204,
  ResetContent                 = 205,
  PartialContent               = 206,

  // 3xx: Redirection
  MultipleChoices              = 300,
  MovedPermanently             = 301,
  Found                        = 302,
  SeeOther                     = 303,
  NotModified                  = 304,
  UseProxy                     = 305,
  TemporaryRedirect            = 307,

  // 4xx: Client Error
  BadRequest                   = 400,
  Unauthorized                 = 401,
  PaymentRequired              = 402,
  Forbidden                    = 403,
  NotFound                     = 404,
  MethodNotAllowed             = 405,
  NotAcceptable                = 406,
  ProxyAuthenticationRequired  = 407,
  RequestTimeOut               = 408,
  Conflict                     = 409,
  Gone                         = 410,
  LengthRequired               = 411,
  PreconditionFailed           = 412,
  RequestEntityTooLarge        = 413,
  RequestURITooLarge           = 414,
  UnsupportedMediaType         = 415,
  RequestedRangeNotSatisfiable = 416,
  ExpectationFailed            = 417,

  // 5xx: Server Error
  InternalServerError          = 500,
  NotImplemented               = 501,
  BadGateway                   = 502,
  ServiceUnavailable           = 503,
  GatewayTimeOut               = 504,
  HTTPVersionNotSupported      = 505
};

/**
 * \brief Enumeration containing the various HTTP status code categories.
 */
enum Category {
  UndefinedCategory     = 0,
  Informational         = 100,
  Success               = 200,
  Redirection           = 300,
  ClientError           = 400,
  ServerError           = 500
};

} // ! status_codes

} // ! bref

#endif  // ! BREF_API_HTTPCONSTANTS_H
