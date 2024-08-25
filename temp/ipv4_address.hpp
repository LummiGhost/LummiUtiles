#pragma once

#include <cctype>
#include <string>
#include <iostream>
#include <sstream>

#include "Text_Serialization.h"


using namespace std;
using namespace Stun::Serialization::Text;


namespace Stun{
	namespace Network {
		class IPv4Address
		{
			uint8_t address[4];
		public:
			IPv4Address() :
				address{ 0, 0, 0, 0 }
			{}
			IPv4Address(const std::string& str) :
				IPv4Address()
			{
				load(str);
			}
			IPv4Address(const IPv4Address& other) :
				address{ other.address[0], other.address[1], other.address[2], other.address[3] }
			{}
			IPv4Address(IPv4Address&& other) noexcept :
				address{ other.address[0], other.address[1], other.address[2], other.address[3] }
			{}

			std::string to_string() const
			{
				std::string str;
				str.reserve(15);
				str.append(std::to_string(address[0]));
				str.append(".");
				str.append(std::to_string(address[1]));
				str.append(".");
				str.append(std::to_string(address[2]));
				str.append(".");
				str.append(std::to_string(address[3]));
				return str;
			}
			std::ostream& print(std::ostream& out) const
			{
				out << to_string();
				return out;
			}

			bool operator==(const IPv4Address& other) const
			{
				return address[0] == other.address[0] &&
					address[1] == other.address[1] &&
					address[2] == other.address[2] &&
					address[3] == other.address[3];
			}
			bool operator < (const IPv4Address& other) const
			{
				if (address[0] < other.address[0] &&
					address[1] < other.address[1] &&
					address[2] < other.address[2] &&
					address[3] < other.address[3])
				{
					return true;
				}
				return false;
			}

			IPv4Address& operator=(const std::string& str)
			{
				load(str);
				return *this;
			}
			IPv4Address& operator=(const IPv4Address& other)
			{
				address[0] = other.address[0];
				address[1] = other.address[1];
				address[2] = other.address[2];
				address[3] = other.address[3];
				return *this;
			}
			IPv4Address operator& (const IPv4Address& other) const
			{
				IPv4Address result;
				result.address[0] = address[0] & other.address[0];
				result.address[1] = address[1] & other.address[1];
				result.address[2] = address[2] & other.address[2];
				result.address[3] = address[3] & other.address[3];
				return result;
			}
			IPv4Address operator| (const IPv4Address& other) const
			{
				IPv4Address result;
				result.address[0] = address[0] | other.address[0];
				result.address[1] = address[1] | other.address[1];
				result.address[2] = address[2] | other.address[2];
				result.address[3] = address[3] | other.address[3];
				return result;
			}
			IPv4Address operator^ (const IPv4Address& other) const
			{
				IPv4Address result;
				result.address[0] = address[0] ^ other.address[0];
				result.address[1] = address[1] ^ other.address[1];
				result.address[2] = address[2] ^ other.address[2];
				result.address[3] = address[3] ^ other.address[3];
				return result;
			}

			friend std::ostream& operator << (std::ostream& out, const IPv4Address& obj)
			{
				return obj.print(out);
			}
			friend std::istream& operator >> (std::istream& in, IPv4Address& obj)
			{
				std::string str;
				in >> str;
				obj = str;
				return in;
			}

		protected:
			void load(const std::string& str)
			{
				size_t pos = 0;
				for (int i = 0; i < 4; i++)
				{
					size_t next = str.find('.', pos);
					if (next == string::npos)
					{
						address[i] = stoi(str.substr(pos));
						break;
					}
					address[i] = stoi(str.substr(pos, next - pos));
					pos = next + 1;
				}
			}
		};
	}
}