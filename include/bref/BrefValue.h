/**
 * \file   BrefValue.h
 * \author Vincent Giersch <vincent.giersch@epitech.eu>
 * \date   Mon Mar 12 07:08:33 2012
 *
 * \brief  BrefValue declaration.
 *
 */

#ifndef _BREF_API_BREFVALUE_H_
#define _BREF_API_BREFVALUE_H_

#pragma once

#include "detail/BrefDLL.h"
#include <list>
#include <map>
#include <string>

namespace bref {

// Forward declaration of the BrefValue class
class BREF_DLL BrefValue;

/**
 * \brief Describe a list of BrefValue.
 *
 * \note This is also a bref value in itself.
 */
typedef std::list<BrefValue>             BrefValueList;

/**
 * \brief Describe an associative array of \c std::string - \c BrefValue.
 *
 * \note This is also a bref value in itself.
 */
typedef std::map<std::string, BrefValue> BrefValueArray;

/**
 * \brief Contains a generic value
 *
 * Usage example:
\code
bref::BrefValue v(42);          // an integer
bref::BrefValue empty;          // a null value

if (v.isInt())
  std::cout << v.asInt() << std::endl;  // 42 is displayed

if (empty.isInt()) {
  std::cout << empty.asInt() << std::endl; // nothing is displayed
}

// An empty string "" is returned (if the value is not of a given
// type, a defaut constructed value is returned).
std::cout << v.asString() << std::endl;
\endcode
 */

class BREF_DLL BrefValue
{
  public:
  /**
   * \brief Config types
   */
  enum confType
    {
      nullType,                 /**< A null value */
      stringType,               /**< The value is a std::string */
      boolType,                 /**< The value is a boolean */
      intType,                  /**< The value is an integer */
      doubleType,               /**< The value is a double */
      listType,                 /**< The value is a ::BrefValueList */
      arrayType                 /**< The value is a ::BrefValueArray */
    };

  /**
   * \brief Default constructor, build a Null BrefValue.
   */
  BrefValue();

  /**
   * \brief Build a BrefValue with a bool.
   */
  BrefValue(bool);

  /**
   * \brief Build a BrefValue with a string.
   */
  BrefValue(const std::string &);

  /**
   * \brief Build a BrefValue with an integer.
   */
  BrefValue(int);

  /**
   * \brief Build a BrefValue with a double.
   */
  BrefValue(double);

  /**
   * \brief Build a BrefValue with a BrefValueArray
   */
  BrefValue(const BrefValueArray &);

  /**
   * \brief Build a BrefValue with a BrefValueList
   */
  BrefValue(const BrefValueList &);

  /**
   * \brief Gets the type of the value
   */

  confType getType() const;

  /**
   * \brief Clear Node content
   */
  void clear();

  /**
   * \brief Test if value is null
   */
  bool isNull() const;

  /**
   * \brief Test if value is a string
   */
  bool isString() const;

  /**
   * \brief Test if value is a boolean
   */
  bool isBool() const;

  /**
   * \brief Test if value is an integer
   */
  bool isInt() const;

  /**
   * \brief test if valuer is a double
   */
  bool isDouble() const;

  /**
   * \brief Test if value is a list
   */
  bool isList() const;

  /**
   * \brief Test if value is an array
   */
  bool isArray() const;

  /**
   * \brief If it's a string, get the value
   */
  const std::string & asString() const;

  /**
   * \brief If it's a boolean, get the value
   */
  bool asBool() const;

  /**
   * \brief If it's an integer, get the value
   */
  int asInt() const;

  /**
   * \brief If it's a double, get the value
   */
  double asDouble() const;

  /**
   * \brief If it's a list, get the value
   */
  const BrefValueList & asList() const;

  /**
   * \brief If it's an array, get the value
   */
  const BrefValueArray & asArray() const;

  /**
   * \brief Check if key exists in array
   * \param key checked array key
   */
  bool hasKey(std::string const & key) const;

  /**
   * \brief Access to array element. Proxy method to arrayValue_.
   */
  BrefValue & operator[](std::string const & key);

  /**
   * \brief Push element in list. Proxy method to listValue_.push_back.
   */
  void push(const bref::BrefValue &node);

  /**
   * \brief Set the content as null
   */
  void setNull();

  /**
   * \brief Set a string as content
   */
  void setString(std::string const &);

  /**
   * \brief Set a boolean as content
   */
  void setBool(bool);

  /**
   * \brief Set integer as content
   */
  void setInt(int);

  /**
   * \brief Set a double as content
   */
   void setDouble(double);

private:
  /**
   * \brief Contains type of Node
   */
  confType type_;

  /**
   * \brief Contains node string
   */
  std::string stringValue_;

  /**
   * \brief Contains node boolean
   */
  bool boolValue_;

  /**
   * \brief Contains node double
   */
  double doubleValue_;

  /**
   * \brief Contains node integer
   */
  int intValue_;

  /**
   * \brief Contains node array
   */
  BrefValueArray arrayValue_;

  /**
   * \brief Contains node list
   */
  BrefValueList listValue_;
};

} // ! bref

#endif /* ! _BREF_API_BREFVALUE_H_ */
