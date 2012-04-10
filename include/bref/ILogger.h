/**
 * \file   ILogger.h
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Sat Mar 10 15:42:33 2012
 *
 * \brief  Déclaration de l'interface ILogger.
 *
 *
 */

#ifndef BREF_API_ILOGGER_H
#define BREF_API_ILOGGER_H

#include <string>

namespace bref {

/**
 * \brief Bref API logger interface.
 *
 * General interface for the logging in modules and the server more
 * generally.
 *
 * \sa ScopedLogger, LOG(logger, severity)
 *
 */
class ILogger
{
public:
  /**
   * \brief Logger severities.
   */
  enum Severity {
    Debug,      /**< debug information mainly for developers */
    Info,       /**< information that can be useful to know about an operation */
    Warning,    /**< a warning */
    Error,      /**< a non-fatal error */
    Fatal       /**< an fatal error that cause the program to stop */
  };

  /**
   * \brief Virtual destructor.
   */
  virtual ~ILogger() { }

  /**
   * \brief Return the severity of the logger.
   *
   * \return The current severity.
   *
   * \sa setSeverity()
   */
  virtual Severity severity() const = 0;

  /**
   * \brief Set the logger severity.
   *
   * \param [in] newSeverity
   *    The new severity for the logger.
   *
   * \sa severity()
   */
  virtual void setSeverity(Severity newSeverity) = 0;

  /**
   * \brief Log a message with a given severity.
   *
   * \note The message is logged only if \p messageSeverity is
   *       superior or equal to \c severity() (the logger severity).
   *
   * \param messageSeverity
   *    The severity of the message.
   * \param message
   *    The message (string) to log.
   */
  virtual void log(Severity messageSeverity, const std::string & message) = 0;
};

} // ! bref

#endif /* !BREF_API_ILOGGER_H */
