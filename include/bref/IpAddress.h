/**
 * \file   IpAddress.h
 * \author Thomas Badaud <thomas.badaud@epitech.eu>
 * \date   Mon Mar 12 06:47:59 2012
 *
 * \brief  IpAddress defintions.
 *
 */

#ifndef BREF_API_IPADDRESS_H_
#define BREF_API_IPADDRESS_H_

#include <stdint.h>

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
# include <windows.h>
#endif

namespace bref {

#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
typedef SOCKET SocketType;
#else
typedef int SocketType;
#endif

/**
 * \brief Simple union that contains an IPv4 address
 */
union IPv4Address
{
  unsigned char bytes[4];
  uint32_t      host;
}; // union 'IPv4Address'

/**
 * \brief Simple union that contains an IPv6 address
 */
union IPv6Address {
  struct {
    union {
      unsigned char networkBytes[8];
      uint64_t      network;
    };
    union {
      unsigned char hostBytes[8];
      uint64_t      host;
    };
  };
  unsigned char     bytes[16];
}; // union 'IPv6Address'

/**
 * \brief Provides IPv4 & Ipv6 encapsulation
 */
class IpAddress {
public:
  IpAddress();
  /**
   * \param get an IpAddress from a hostname, an IPv4 or IPv6 address
   */
  IpAddress(const char * host);
  ~IpAddress();

  /**
   * \brief checks if an address is an IPv4 address
   */
  bool isV4() const;
  /**
   * \brief checks if an address is an IPv6 address
   */
  bool isV6() const;
  /**
   * \brief checks if an address is an IPv4 compatible address
   */
  bool isV4Compatible() const;

  /**
   * \brief get the IPv4 address
   */
  const IPv4Address & getV4() const;
  /**
   * \brief get the IPv6 address
   */
  const IPv6Address & getV6() const;

protected:
  enum { IPv4, IPv6, IPerror } ipAddressStatus_;
  union {
    IPv4Address v4_[4];
    IPv6Address v6_;
  } ipAddress_;
}; // class 'IpAddress'

} // namespace 'bref'

#endif // BREF_API_IPADDRESS_H_
