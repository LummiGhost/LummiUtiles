#pragma once


#include <iostream>
#include <cstdint>
#include <type_traits>
//#include <WinSock2.h>


namespace Stun {
    namespace Network {
 
/// 根据目标计算机架构，推算使用的大小端模式
        enum class Endian {
            Big,
			Little
        };
#if defined(x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || defined(_M_X64) || defined(_M_AMD64)
#define _Endiam Endian::Little
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define _Endiam Endian::Little
#elif defined(__arm__) || defined(__thumb__) || defined(_M_ARM)
#define _Endiam Endian::Little
#elif defined(__Endiam__)
#define _Endiam __Endiam__
#else
#error "Unsupported architecture, please define __Endiam__."
#endif
		constexpr Endian _Currect_System_Endiam = _Endiam;

		/// 用于将大小端模式相互转换
		template<typename T>
		inline T __SwitchBigAndLittle(T v) {
#if defined(__cpp_if_constexpr) && __cpp_if_constexpr >= 201606
			if constexpr (_Currect_System_Endiam == Endian::Big) {
#else
			if (_Currect_System_Endiam == Endian::Big) {
#endif
				return v;
			}
			else {
				T result = 0;
				uint8_t* src = reinterpret_cast<uint8_t*>(&v);
				uint8_t* dest = reinterpret_cast<uint8_t*>(&result);
				for (size_t i = 0; i < sizeof(T); i++) {
					dest[i] = src[sizeof(T) - i - 1];
				}
				return result;
			}
		}

		//using htons = uint16_t(*)(uint16_t);
		//using htonl = uint32_t(*)(uint32_t);
		//using htonll = uint64_t(*)(uint64_t);
		//using ntohs = uint16_t(*)(uint16_t);
		//using ntohl = uint32_t(*)(uint32_t);
		//using ntohll = uint64_t(*)(uint64_t);

		inline uint16_t htons(uint16_t v) {
			return __SwitchBigAndLittle(v);
		}
		inline uint32_t htonl(uint32_t v) {
			return __SwitchBigAndLittle(v);
		}
		inline uint64_t htonll(uint64_t v) {
			return __SwitchBigAndLittle(v);
		}
		inline uint16_t ntohs(uint16_t v) {
			return __SwitchBigAndLittle(v);
		}
		inline uint32_t ntohl(uint32_t v) {
			return __SwitchBigAndLittle(v);
		}
		inline uint64_t ntohll(uint64_t v) {
			return __SwitchBigAndLittle(v);
		}

    } // Network
} // Stun

