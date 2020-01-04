#include "IpAddress.h"
#include "utils.h"
#include <boost/xpressive/xpressive.hpp>
#include <iostream>
#include <regex>

namespace bxp = boost::xpressive;

static const std::string RegExIpV4_IpFormatHost = "^[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]{1,3}\\.[0-9]+(\\:[0-9]{1,5})?$";
static const std::string RegExIpV4_StringHost = "^[A-Za-z0-9]+(\\:[0-9]+)?$";
static const std::string RegExIpV4_StringNode = "^([A-Za-z0-9-]+\\.)+[A-Za-z0-9-]+(\\:[0-9]+)?$";
static const std::string RegExIpV6_IpFormatHost = "(([0-9a-fA-F]{1,4}:){7,7}[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,7}:|([0-9a-fA-F]{1,4}:){1,6}:[0-9a-fA-F]{1,4}|([0-9a-fA-F]{1,4}:){1,5}(:[0-9a-fA-F]{1,4}){1,2}|([0-9a-fA-F]{1,4}:){1,4}(:[0-9a-fA-F]{1,4}){1,3}|([0-9a-fA-F]{1,4}:){1,3}(:[0-9a-fA-F]{1,4}){1,4}|([0-9a-fA-F]{1,4}:){1,2}(:[0-9a-fA-F]{1,4}){1,5}|[0-9a-fA-F]{1,4}:((:[0-9a-fA-F]{1,4}){1,6})|:((:[0-9a-fA-F]{1,4}){1,7}|:)|fe80:(:[0-9a-fA-F]{0,4}){0,4}%[0-9a-zA-Z]{1,}|::(ffff(:0{1,4}){0,1}:){0,1}((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])|([0-9a-fA-F]{1,4}:){1,4}:((25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9])\\.){3,3}(25[0-5]|(2[0-4]|1{0,1}[0-9]){0,1}[0-9]))";


IpAddress::IpAddress(void)
:m_type(Unknown)
,m_portNumber(0)
{
}

IpAddress::~IpAddress(void)
{
}

IpAddress IpAddress::Parse( const std::string& ip_address_str )
{
    IpAddress ipaddress;
    bxp::sregex ip_regex = bxp::sregex::compile(RegExIpV4_IpFormatHost);
    bxp::sregex str_regex = bxp::sregex::compile(RegExIpV4_StringHost);
    bxp::sregex node_regex = bxp::sregex::compile(RegExIpV4_StringNode);
    bxp::sregex ip6_regex = bxp::sregex::compile(RegExIpV6_IpFormatHost);
    bxp::smatch match;

    if (bxp::regex_match(ip_address_str, match, ip_regex) ||
        bxp::regex_match(ip_address_str, match, str_regex) ||
        bxp::regex_match(ip_address_str, match, node_regex))
    {
        ipaddress.m_type = IpV4;
        // Anything before the last ':' (if any) is the host address
        std::string::size_type colon_index = ip_address_str.find_last_of(':');
        if (std::string::npos == colon_index)
        {
            ipaddress.m_portNumber = 0;
            ipaddress.m_hostAddress = ip_address_str;
        }else{
            ipaddress.m_hostAddress = ip_address_str.substr(0, colon_index);
            int parsed = std::stoi(ip_address_str.substr(colon_index+1));
            check_port(parsed);
            ipaddress.m_portNumber = parsed;
        }
    }

    std::regex ipv6_pattern(RegExIpV6_IpFormatHost);
    std::cmatch m;

    if (std::regex_match (ip_address_str.c_str(), m, ipv6_pattern))
    {
      ipaddress.m_type = IpV6;
      ipaddress.m_portNumber = 0;
      ipaddress.m_hostAddress = ip_address_str;
    } else {
      std::string::size_type colon_index = ip_address_str.find_last_of(':');
      std::string possible = ip_address_str.substr(0, colon_index);
      std::regex_match (possible.c_str(), m, ipv6_pattern);

      if (std::regex_match (possible.c_str(), m, ipv6_pattern)) {
        ipaddress.m_type = IpV6;
        ipaddress.m_hostAddress = possible;
        int parsed = std::stoi(ip_address_str.substr(colon_index+1));
        check_port(parsed);
        ipaddress.m_portNumber = parsed;
      }
    }

    return ipaddress;
}

std::string IpAddress::TypeToString( Type address_type )
{
    std::string result = "Unknown";
    switch(address_type)
    {
    case IpV4:
        result = "IP Address Version 4";
        break;
    case IpV6:
        result = "IP Address Version 6";
        break;
    default:
        break;
    }
    return result;
}

const std::string& IpAddress::getHostAddress() const
{
    return m_hostAddress;
}

unsigned short IpAddress::getPortNumber() const
{
    return m_portNumber;
}

IpAddress::Type IpAddress::getType() const
{
    return m_type;
}
