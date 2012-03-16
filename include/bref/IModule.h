/**
 * \file   IModule.h
 * \author Florent Chiron <florent.chiron@epitech.eu>
 * \date   Sun Mar 11 00:00:00 2012
 *
 * \brief  IModule declaration.
 *
 */

#ifndef ZIA_SRC_API_IMODULE_H
#define ZIA_SRC_API_IMODULE_H

#pragma once

#include <string>

#include "Version.h"
#include "Pipeline.h"

namespace bref
{
/**
 * \brief Class used to operate with dynamically loaded modules.
 *
 *        Each module must provide a function to allow its loading at runtime,
 *        matching the following prototype :
 *        \code extern "C" IModule * loadModule(); \endcode
 *
 *        After the module has been loaded, a version check should be performed as follows :
 *        If the major numbers of the API version and of the minimum API version supported by the
 *        module do not match, the module is not used.
 *        If the major numbers match, but the minor numbers do not, the module is used but a
 *        warning is emitted.
 */
class IModule
{
public:
    /**
     * \brief Virtual destructor for the module.
     */
    virtual ~IModule() { }

    /**
     * \brief Retrieve the module's name.
     *
     * \return const std::string &  The module's name.
     */
    virtual const std::string & name() const = 0;

    /**
     * \brief Retrieve the module's description.
     *
     * \return const std::string &  The module's description.
     */
    virtual const std::string & description() const = 0;

    /**
     * \brief Retrieve the module's version.
     *
     * \param[out] minor The module's version.
     */
    virtual Version version() const = 0;

    /**
     * \brief Retrieve the minimum API version the module is compatible with.
     *
     * \param[out] version The minimum API version.
     */
    virtual Version minimumApiVersion() const = 0;

    /**
     * \brief Register the module hooks on the convenient hookpoints of the pipeline.
     *
     * \param[out] pipeline The pipeline.
     */
    virtual void registerHooks(Pipeline & pipeline) = 0;

    /**
     * \brief Dispose of the resources allocated by the module.
     */
    virtual void dispose() = 0;
};

}

#endif // ZIA_SRC_API_IMODULE_H
