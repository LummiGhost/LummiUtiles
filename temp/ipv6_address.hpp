#pragma once


#include <cctype>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "Text_Serialization.h"


using namespace std;
using namespace Stun::Serialization::Text;


namespace Stun {
	namespace Network {
		class IPv6Address
		{
			uint16_t address[8];
		public:
			IPv6Address() :
				address{ 0, 0, 0, 0, 0, 0, 0, 0 }
			{}
			IPv6Address(const std::string& str) :
				IPv6Address()
			{
				load(str);
			}
			IPv6Address(const IPv6Address& other) :
				address{ 
				other.address[0], 
				other.address[1], 
				other.address[2], 
				other.address[3], 
				other.address[4], 
				other.address[5], 
				other.address[6], 
				other.address[7] }
			{}
			IPv6Address(IPv6Address&& other) noexcept :
				address{ 
				other.address[0], 
				other.address[1], 
				other.address[2], 
				other.address[3], 
				other.address[4], 
				other.address[5], 
				other.address[6], 
				other.address[7] }
			{}

			std::string to_string() const
			{
				std::stringstream ss;
				for (int i = 0; i < 8; ++i) {
					if (i != 0) ss << ":";
					ss << std::hex << std::setw(4) << std::setfill('0') << address[i];
				}
				return ss.str();
			}
			std::ostream& print(std::ostream& out) const 
			{
				out << to_string();
				return out;
			}

			bool operator==(const IPv6Address& other) const
			{
				return address[0] == other.address[0] &&
					address[1] == other.address[1] &&
					address[2] == other.address[2] &&
					address[3] == other.address[3] &&
					address[4] == other.address[4] &&
					address[5] == other.address[5] &&
					address[6] == other.address[6] &&
					address[7] == other.address[7];
			}
			bool operator < (const IPv6Address& other) const
			{
				if (address[0] < other.address[0] &&
					address[1] < other.address[1] &&
					address[2] < other.address[2] &&
					address[3] < other.address[3] &&
					address[4] < other.address[4] &&
					address[5] < other.address[5] &&
					address[6] < other.address[6] &&
					address[7] < other.address[7])
				{
					return true;
				}
				return false;
			}
			IPv6Address& operator=(const std::string& str)
			{
				load(str);
				return *this;
			}
			IPv6Address& operator=(const IPv6Address& other)
			{
				address[0] = other.address[0];
				address[1] = other.address[1];
				address[2] = other.address[2];
				address[3] = other.address[3];
				address[4] = other.address[4];
				address[5] = other.address[5];
				address[6] = other.address[6];
				address[7] = other.address[7];
				return *this;
			}
			IPv6Address operator& (const IPv6Address& other) const
			{
				IPv6Address result;
				result.address[0] = address[0] & other.address[0];
				result.address[1] = address[1] & other.address[1];
				result.address[2] = address[2] & other.address[2];
				result.address[3] = address[3] & other.address[3];
				result.address[4] = address[4] & other.address[4];
				result.address[5] = address[5] & other.address[5];
				result.address[6] = address[6] & other.address[6];
				result.address[7] = address[7] & other.address[7];
				return result;
			}
			IPv6Address operator| (const IPv6Address& other) const
			{
				IPv6Address result;
				result.address[0] = address[0] | other.address[0];
				result.address[1] = address[1] | other.address[1];
				result.address[2] = address[2] | other.address[2];
				result.address[3] = address[3] | other.address[3];
				result.address[4] = address[4] | other.address[4];
				result.address[5] = address[5] | other.address[5];
				result.address[6] = address[6] | other.address[6];
				result.address[7] = address[7] | other.address[7];
				return result;
			}
			IPv6Address operator^ (const IPv6Address& other) const
			{
				IPv6Address result;
				result.address[0] = address[0] ^ other.address[0];
				result.address[1] = address[1] ^ other.address[1];
				result.address[2] = address[2] ^ other.address[2];
				result.address[3] = address[3] ^ other.address[3];
				result.address[4] = address[4] ^ other.address[4];
				result.address[5] = address[5] ^ other.address[5];
				result.address[6] = address[6] ^ other.address[6];
				result.address[7] = address[7] ^ other.address[7];
				return result;
			}

			friend std::ostream& operator << (std::ostream& out, const IPv6Address& obj)
			{
				return obj.print(out);
			}
			friend std::istream& operator >> (std::istream& in, IPv6Address& obj)
			{
				std::string str;
				in >> str;
				obj = str;
				return in;
			}

		protected:
			void load(const std::string& str) 
			{
				stringstream ss;
				ss << str;
				ss >> hex;
				for (int i = 0; i < 8; i++)
				{
					ss >> address[i];
					if (ss.eof())
					{
						break;
					}
					if (ss.peek() == ':')
					{
						ss.ignore();
					}
				}
			}
		};
	}
}