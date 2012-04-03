/**
 * \file   AModule.h
 * \author Florent Chiron <florent.chiron@epitech.eu>
 * \date   Sun Mar 11 00:00:00 2012
 *
 * \brief  AModule declaration.
 *
 */

#ifndef ZIA_SRC_API_AMODULE_H
#define ZIA_SRC_API_AMODULE_H

#pragma once

#include <string>

#include "Version.h"
#include "Pipeline.h"

namespace bref
{
/**
 * \brief Class used to operate with dynamically loaded modules.
 *
 * Each module must provide a function to allow its loading at
 * runtime, matching the following prototype:
 *
 * \code
extern "C" bref::AModule *loadModule(bref::ILogger *,
                                     const bref::ServerConfig &,
                                     const bref::IConfHelper &);
\endcode
 *
 * Once the module is loaded, a version check should be performed as
 * follows:
 *
 * - If the major number of the API doesn't match with the major
 *   number of the minimum Api version supported by the module. The
 *   module is not used and a warning is emitted.
 *
 * - If the major numbers match, but the minor numbers do not, the
 *   module is used but a warning is emitted.
 *
 */
class AModule
{
protected:
    /**
     * \brief AModule constructor, which defines module's informations
     *
     * \param name Contains module's name
     * \param description Contains module's description
     * \param version Contain's module's version
     * \param minimumApiVersion Required API version to use the module
     */

    AModule(const std::string & name,
            const std::string & description,
            const Version & version,
            const Version & minimumApiVersion);

    /**
     * \brief Virtual destructor for the module.
     *
     * The \c delete() operator should not be called on the AModule
     * instance. Please refer to the \c dispose() method.
     *
     * \note The destructor is protected in order to disable the call
     *       to the \c delete() operator from the server code.
     *
     * \sa dispose()
     */
    virtual ~AModule() { }

    /**
     * \brief Contains module's name
     * \sa name()
     */
    std::string name_;

    /**
     * \brief Contains module's description
     * \sa description()
     */
    std::string description_;

    /**
     * \brief Contains module version
     * \sa version()
     */
    Version version_;

    /**
     * \brief Minimum API version
     * \sa minimumApiVersion()
     */
    Version minimumApiVersion_;

public:
    /**
     * \brief Retrieve the module's name.
     *
     * \return The module's name.
     */
    virtual const std::string & name() const;

    /**
     * \brief Retrieve the module's description.
     *
     * \return The module's description.
     */
    virtual const std::string & description() const;

    /**
     * \brief Retrieve the module's version.
     *
     * \return The module's version.
     */
    virtual const Version & version() const;

    /**
     * \brief Retrieve the minimum API version the module is compatible with.
     *
     * \return The minimum API version supported by this module.
     */
    virtual const Version & minimumApiVersion() const;

    /**
     * \brief Register the module hooks on the convenient hookpoints
     *        of the pipeline.
     *
     *        Example:
     *
\code
void registerHooks(Pipeline & pipeline)
{
  const float lowPriority = 0.5f;

  pipeline.connectionHooks.push_back(std::make_pair(&hook_function,
                                                    lowPriority));
}
\endcode
     *
     * \param[out] pipeline The pipeline.
     */
    virtual void registerHooks(Pipeline & pipeline) = 0;

    /**
     * \brief Dispose of the resources allocated by the module.
     *
     * \note This is often represented by a simple:
     *       \code delete this; \endcode
     */
    virtual void dispose() = 0;
};

} // ! bref

#endif // ZIA_SRC_API_AMODULE_H
