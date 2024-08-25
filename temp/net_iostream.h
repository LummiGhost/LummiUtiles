#pragma once

#include <iostream>
#include <string>
#include <sstream>

#include "NetPKG.h"
#include "Bin_Serialization_interface.h"
//#include "Bin_Serialization.h"

//namespace Stun {
//	namespace Serialization {
//		namespace Bin {
//			struct IDumpable;
//			struct ILoadable;
//			struct IPackageable;
//		}
//	}
//}

using namespace Stun::Serialization::Bin;
using namespace Stun::Network;


namespace Stun {
	namespace Network {
		using bin_ostream = std::basic_ostream<uint8_t>;
		using bin_istream = std::basic_istream<uint8_t>;
		using bin_iostream = std::basic_iostream<uint8_t>;

		using bin_streambuf = std::basic_streambuf<uint8_t>;

		template<typename _BufferType>
		class Net_IOStream :
			public std::basic_iostream<_BufferType>
		{
		public:
			Net_IOStream(std::basic_streambuf<_BufferType>* buf) :
				std::basic_iostream<_BufferType>(buf)
			{}
			Net_IOStream(std::ostringstream oss):
				std::basic_iostream<_BufferType>(oss.rdbuf())
			{}
			Net_IOStream(std::stringstream ss):
				std::basic_iostream<_BufferType>(ss.rdbuf())
			{}
			
			template<typename T, typename = typename std::enable_if<std::is_base_of<IDumpable, T>::value>::type>
			friend Net_IOStream& operator << (Net_IOStream& out, const T& obj) {
				return obj.dump(out);
			}

			template<typename T, typename = typename std::enable_if<std::is_base_of<ILoadable, T>::value>::type>
			friend Net_IOStream& operator >> (Net_IOStream& in, T& obj) {
				return obj.load(in);
			}

			template<typename T>
			Net_IOStream& write(const T* ptr, std::streamsize count) {
				//this->std::basic_iostream<_BufferType>::write(reinterpret_cast<const _BufferType*>(ptr), count);
				for (size_t i = 0; i < count / sizeof T; i++) {
					this->put(ptr[i]);
				}
				return *this;
			}
			template<typename T>
			Net_IOStream& read(T* ptr, std::streamsize count) {
				//this->std::basic_iostream<_BufferType>::read(reinterpret_cast<_BufferType*>(ptr), count);
				for (size_t i = 0; i < count / sizeof T; i++) {
					this->get(ptr[i]);
				}
				return *this;
			}

			Net_IOStream& put(char obj) {
				this->std::basic_iostream<_BufferType>::put(obj);
				return *this;
			}
			Net_IOStream& put(uint8_t obj) {
				this->std::basic_iostream<_BufferType>::put(obj);
				return *this;
			}
			Net_IOStream& put(uint16_t obj) {
				uint16_t tmp = htons(obj);
				this->std::basic_iostream<_BufferType>::write(reinterpret_cast<_BufferType*>(&tmp), sizeof tmp);
				return *this;
			}
			Net_IOStream& put(uint32_t obj) {
				uint32_t tmp = htonl(obj);
				this->std::basic_iostream<_BufferType>::write(reinterpret_cast<_BufferType*>(&tmp), sizeof tmp);
				return *this;
			}
			Net_IOStream& put(uint64_t obj) {
				uint64_t tmp = htonll(obj);
				this->std::basic_iostream<_BufferType>::write(reinterpret_cast<_BufferType*>(&tmp), sizeof tmp);
				return *this;
			}
			Net_IOStream& put(const std::string& obj) {
				this->std::basic_iostream<_BufferType>::write(reinterpret_cast<const _BufferType*>(obj.c_str()), obj.size());
				return *this;
			}
			Net_IOStream& put(const IDumpable& obj) {
				return (&obj)->dump(*this);
			}
			/// NOTE: 多个类同时实现 IDumpable 和 IPrintable 导致调用不明确，方便起见，删除了这个重载。
			//Net_IOStream& put(const IPrintable& obj) {
			//	return obj.print(*this);
			//}

			Net_IOStream& get(char& obj) {
				this->std::basic_iostream<_BufferType>::read(reinterpret_cast<_BufferType*>(&obj), sizeof obj);
				return *this;
			}
			Net_IOStream& get(uint8_t& obj) {
				this->std::basic_iostream<_BufferType>::read(reinterpret_cast<_BufferType*>(&obj), sizeof obj);
				return *this;
			}
			Net_IOStream& get(uint16_t& obj) {
				uint16_t tmp = 0;
				this->std::basic_iostream<_BufferType>::read(reinterpret_cast<_BufferType*>(&tmp), sizeof tmp);
				obj = ntohs(tmp);
				return *this;
			}
			Net_IOStream& get(uint32_t& obj) {
				uint32_t tmp = 0;
				this->std::basic_iostream<_BufferType>::read(reinterpret_cast<_BufferType*>(&tmp), sizeof tmp);
				obj = ntohl(tmp);
				return *this;
			}
			Net_IOStream& get(uint64_t& obj) {
				uint64_t tmp = 0;
				this->std::basic_iostream<_BufferType>::read(reinterpret_cast<_BufferType*>(&tmp), sizeof tmp);
				obj = ntohll(tmp);
				return *this;
			}
			Net_IOStream& get(std::string& obj) {
				std::streamsize size = obj.size();
				this->std::basic_iostream<_BufferType>::read(reinterpret_cast<_BufferType*>(&obj[0]), size);
				return *this;
			}
			Net_IOStream& get(ILoadable& obj) {
				return (& obj)->load(*this);
			}

			Net_IOStream& ignore(std::streamsize count) {
				this->std::basic_iostream<_BufferType>::ignore(count);
				return *this;
			}
		};

		using Bin_Net_IOStream = Net_IOStream<uint8_t>;
		using Text_Net_IOStream = Net_IOStream<char>;
		using WText_Net_IOStream = Net_IOStream<wchar_t>;

	}///namespace Network
}///namespace Stun