#ifndef _BREF_UTIL_NONCOPYABLE_
#define _BREF_UTIL_NONCOPYABLE_
#pragma once

namespace bref {
namespace util {

/**
 * \brief Basic non copyable class, using private copying methods
 */
class NonCopyable
{
protected:
  NonCopyable()
  { }

  ~NonCopyable()
  { }

private:
  NonCopyable(const NonCopyable &);
  NonCopyable & operator=(const NonCopyable &);
};

} // ! util
} // ! bref

#endif /* !_BREF_UTIL_NONCOPYABLE_ */
