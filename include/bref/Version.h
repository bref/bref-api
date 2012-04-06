/**
 * \file   Version.h
 * \author Guillaume Papin <guillaume.papin@epitech.eu>
 * \date   Mon Mar 12 07:44:08 2012
 *
 * \brief  Version struct definition.
 *
 *
 */

#ifndef BREF_API_VERSION_H_
#define BREF_API_VERSION_H_

namespace bref {

/**
 * \brief Describe a version with a minor and a major number.
 */
struct Version
{
  Version()
    : Major(0),
      Minor(0)
  { }

  Version(int theMajor, int theMinor)
    : Major(theMajor),
      Minor(theMinor)
  { }

  int Major;
  int Minor;
};

} // ! bref

#endif /* !BREF_API_VERSION_H_ */
