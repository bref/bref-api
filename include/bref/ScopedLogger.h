/**
 * \file   ScopedLogger.h
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Sat Mar 10 21:29:54 2012
 *
 * \brief  ScopedLogger class and associated macros definitions.
 *
 *
 */

#ifndef BREF_API_SCOPEDLOGGER_H
#define BREF_API_SCOPEDLOGGER_H

#include <sstream>

#include "ILogger.h"
#include "detail/util/NonCopyable.hpp"

/**
 * \brief A macro to get a logging stream of a given severity.
 *
 * This macros check the current level of the logger and the stream is
 * used only in this case. This is useful to limit the cost of the
 * stream operations, by creating the stream only when needed.
 *
 * Example:
\code
LOG(logger, bref::ILogger::Debug) << "new connection from " << host;
\endcode
 *
 * \sa LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL
 *
 */
#define LOG(logger, sev)                                                \
  if (sev < logger->severity())                                         \
    { }                                                                 \
  else                                                                  \
    bref::ScopedLogger(logger, sev).log()

/**
 * \brief Get the debug stream.
 *
 * \sa bref::ILogger::Debug
 */
#define LOG_DEBUG(logger) LOG(logger, bref::ILogger::Debug)

/**
 * \brief Get the stream to log an information.
 *
 * \sa bref::ILogger::Info
 */
#define LOG_INFO(logger)  LOG(logger, bref::ILogger::Info)

/**
 * \brief Get the stream to log a warning.
 *
 * \sa bref::ILogger::Warning
 */
#define LOG_WARN(logger)  LOG(logger, bref::ILogger::Warning)

/**
 * \brief Get the stream to log an error.
 *
 * \sa bref::ILogger::Error
 */
#define LOG_ERROR(logger) LOG(logger, bref::ILogger::Error)

/**
 * \brief Get the stream to log a fatal message.
 *
 * \sa bref::ILogger::Fatal
 */
#define LOG_FATAL(logger) LOG(logger, bref::ILogger::Fatal)

namespace bref {

/**
 * \brief RAII class for the ILogger interface.
 *
 * \sa ILogger, LOG(logger, severity)
 */
class ScopedLogger : util::NonCopyable
{
private:
  std::stringstream  ss_;
  ILogger           *logger_;
  ILogger::Severity  severity_;

public:

  /**
   * \brief Create a ScopeLogger targetting a given logger and
   *        severity.
   *
   * \param logger
   *            The logger where the message should be written.
   * \param severity
   *            The severity of the message.
   *
   * \sa ~ScopedLogger(), log()
   */
  inline ScopedLogger(ILogger *logger, ILogger::Severity severity)
    : logger_(logger)
    , severity_(severity)
  { }

  /**
   * \brief Write the message to the logger.
   *
   * This is where the stream is actually logged.
   *
   */
  inline ~ScopedLogger()
  {
    logger_->log(severity_, ss_.str());
  }

  /**
   * \brief Retrieve the output stream of the scoped logger.
   *
   * \return A reference to the internal stream of the scoped logger.
   */
  inline std::ostream & log()
  {
    return ss_;
  }
};

} // ! bref

#endif /* !BREF_API_SCOPEDLOGGER_H */
