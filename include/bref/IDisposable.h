/**
 * \file   IDisposable.h
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Sat Apr  7 01:56:32 2012
 *
 * \brief  IDisposable interface declaration.
 *
 */

#ifndef BREF_API_IDISPOSABLE_H_
#define BREF_API_IDISPOSABLE_H_

#include "detail/BrefDLL.h"

namespace bref {

/**
 * \brief Define a dispose() method, useful for resources
 *        de-allocation in a DLL.
 *
 * \sa dispose()
 */
class BREF_DLL IDisposable
{
public:
  /**
   * \brief Dispose allocated resources by the module.
   *
   * Please refer to:
   * - http://chadaustin.me/cppinterface.html (Revision 3)
   * - "the internet" for other reasons
   *
   * \note This is often represented by a simple:
   *       \code delete this; \endcode
   */
  virtual void dispose() = 0;

protected:
  /**
     * \brief Virtual destructor
     *
     * The \c delete() operator should not be called, please refer to
     * the \c dispose() method.
     *
     * \note The destructor is protected in order to disable the call to
     *       the \c delete() operator from the server code.
     *
     * \sa dispose()
     */
    virtual ~IDisposable() { }

};

} // ! bref

#endif /* !BREF_API_IDISPOSABLE_H_ */



