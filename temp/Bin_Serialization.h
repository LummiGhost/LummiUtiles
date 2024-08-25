#pragma once


#include <iostream>
#include <cstdint>
#include <memory>
#include <cctype>
#include <iomanip>

#include "NetPKG.h"
#include "Bin_Serialization_interface.h"
#include "net_iostream.h"


#if defined(__Word__Length)
#define _Word __Word__Length
#elif defined(x86_64) || defined(__x86_64__) || defined(__amd64__) || defined(__amd64) || defined(_M_X64) || defined(_M_AMD64)
#define _Word 8
#elif defined(i386) || defined(__i386__) || defined(__i386) || defined(_M_IX86)
#define _Word 4
#elif defined(__arm__) || defined(__thumb__) || defined(_M_ARM)
#define _Word 4
#else
#message "Unsupported architecture, using `sizeof int`. You can define `__Word__Length` to custom word length."
#define _Word sizeof(int)
#endif

using namespace Stun::Network;

namespace Stun
{
	namespace Serialization
	{
		namespace Bin
		{

			template<typename T>
			class BinOut
			{
				static_assert(std::is_base_of<IDumpable, T>::value, "T must be derived from IDumpable");
			public:
				class Hex_View;

				BinOut(const T* const parent, const int word_length = _Word):
					ptr(parent),
					wordLength(word_length)
				{}
				BinOut(BinOut&& other) :
					ptr(other.ptr),
					wordLength(other.wordLength)
				{
					//ptr = other.ptr;
					//wordLength = other.wordLength;
					other.ptr = nullptr;
				}

				~BinOut() {
					//if (buf) {
					//	delete buf;
					//}
				}

				Hex_View hex_view(const int bytes_in_line = 0) const {
					return Hex_View(this, bytes_in_line ? bytes_in_line : wordLength);
				}

				friend Text_Net_IOStream& operator << (Text_Net_IOStream& out, const BinOut<T>& obj) {
					return obj.ptr->dump(out);
				}
				friend std::ostream& operator << (std::ostream& out, const BinOut<T>& obj) {
					Text_Net_IOStream nios(out.rdbuf());
					obj.ptr->dump(nios);
					return out;
				}

				/// <summary>
				///  Read binary data from a output stream, 
				///  and return a string of binary data.
				/// </summary>
				/// <returns>
				///  Binary Data in string format.
				/// </returns>
				auto data() const {
					std::ostringstream oss(std::ios::binary);
					Text_Net_IOStream nios(oss.rdbuf());
					ptr->dump(nios);
					return oss.str();
				}

			private:
				const T* const ptr;
				const int wordLength;
				//std::streambuf* buf;


			};

			/// DO NOT USE
			template<typename T>
			class enable_BinOut
			{
				static_assert(std::is_base_of<IDumpable, T>::value, "T must be derived from IDumpable");
			public:
				BinOut<T> bin() const{
					return BinOut<T>(this);
				}
			};

			template<typename T>
			class BinIn
			{
				static_assert(std::is_base_of<ILoadable, T>::value, "T must be derived from ILoadable");
			public:
				BinIn(T* const parent) : ptr(parent) {}
				BinIn(BinIn&& other) : ptr(other.ptr) { other.ptr = nullptr; }

				friend Text_Net_IOStream& operator >> (Text_Net_IOStream& in, BinIn<T>& obj) {
					return obj.ptr->load(in);
				}
				friend std::istream& operator >> (std::istream& in, BinIn<T>& obj) {
					Text_Net_IOStream nios(in.rdbuf());
					obj.ptr->load(nios);
					return in;
				}

			private:
				T* const ptr;
			};

			/// DO NOT USE
			template<typename T>
			class enable_BinIn
			{
				static_assert(std::is_base_of<ILoadable, T>::value, "T must be derived from ILoadable");
			public:
				BinIn<T> bin() {
					return BinIn<T>(this);
				}
			};

			template<typename T>
			class BinIO:
				public BinOut<T>,
				public BinIn<T>
			{
				static_assert(std::is_base_of<IPackageable, T>::value, "T must be derived from IPackageable");
			public:
				BinIO(T* const parent)
					: BinOut<T>(parent), BinIn<T>(parent) {}

				BinIO(BinIO&& other)
					: BinOut<T>(std::move(other)), BinIn<T>(std::move(other)) {}
			};

			/// DO NOT USE
			template<typename T, typename = typename std::enable_if<std::is_base_of<IPackageable, T>::value>::type>
			class enable_BinIO:
				public enable_BinOut<T>,
				public enable_BinIn<T>
			{
			public:
				BinIO<T> bin() {
					return BinIO<T>(this);
				}
			};

			template<typename T>
			class BinOut<T>::Hex_View {
				friend class BinOut<T>;

				Hex_View(const BinOut<T>* const parent, const int bytesInLine = _Word) :
					ptr(parent),
					bytes_in_line(bytesInLine)
				{}
				Hex_View(Hex_View&& other) :
					ptr(other.ptr),
					bytes_in_line(other.bytes_in_line)
				{}
				const BinOut<T>* const ptr;
				const int bytes_in_line;
				static char to_printable(uint8_t c) {
					return std::isprint(c) ? c : '.';
				}
				std::ostream& print(std::ostream& out) const {
					/// Output like this:
					/// ```
					/// Hex View of object <0x00001234>: 
					/// ---
					/// | Offset     | Content				   | ASCII	  |
					/// | 0x00000000 | 00 01 02 03 04 05 06 07 | ........ |
					/// | 0x00000008 | 08 09 0A 0B 0C 0D 0E 0F | ........ |
					///  ...
					/// | 0x000000F8 | F8 F9 FA FB FC FD FE FF | ........ |
					/// ---
					/// size: 256 bytes
					/// typeinfo: std::vector<int>
					/// ```

					/// Header
					out << "Hex View of object <" << ptr->ptr << ">: " << std::endl;
					out << "---" << std::setfill(' ') << std::endl;

					out << "| " << std::setw(10) << "Offset"
						<< " | " << std::setw(static_cast<std::streamsize>(3) * bytes_in_line - 1) << "Content"
						<< " | " << std::setw(bytes_in_line) << "ASCII"
						<< " |" << std::endl;

					/// Body
					const size_t size = ptr->ptr->size();
					const size_t lines = size / bytes_in_line;
					const size_t rest = size % bytes_in_line;
					const auto data = ptr->data();

					for (size_t i = 0; i < lines; ++i) {
						/// Offset
						out << "| 0x"
							<< std::setw(8)
							<< std::hex
							<< std::setfill('0')
							<< i * bytes_in_line
							<< " | ";
						/// Content
						for (size_t j = 0; j < bytes_in_line; ++j) {
							out << std::hex
								<< std::setw(2)
								<< std::setfill('0')
								<< ((unsigned int)data[i * bytes_in_line + j] & 0xFF)
								<< " ";
						}
						out << "| ";
						/// ASCII
						for (size_t j = 0; j < bytes_in_line; ++j) {
							out << to_printable(data[i * bytes_in_line + j]);
						}
						out << " |" << std::endl;
					}
					if (rest) {
						/// Offset
						out << "| 0x"
							<< std::setw(8)
							<< std::hex
							<< std::setfill('0')
							<< lines * bytes_in_line
							<< " | ";
						/// Content
						for (size_t j = 0; j < rest; ++j) {
							out << std::hex
								<< std::setw(2)
								<< std::setfill('0')
								<< ((unsigned int)data[lines * bytes_in_line + j] & 0xFF)
								<< " ";
						}
						for (size_t j = rest; j < bytes_in_line; ++j) {
							out << "   ";
						}
						out << "| ";
						/// ASCII
						for (size_t j = 0; j < rest; ++j) {
							out << to_printable(data[lines * bytes_in_line + j]);
						}
						for (size_t j = rest; j < bytes_in_line; ++j) {
							out << " ";
						}
						out << " |" << std::endl;
					}

					/// Footer
					out << "---" << std::endl;
					out << "size: " << std::dec << size << " bytes" << std::endl;
					out << "typeinfo: " << typeid(*ptr->ptr).name() << std::endl;
					return out;
				}
			public:
				friend std::ostream& operator << (std::ostream& out, const Hex_View& obj) {
					return obj.print(out);
				}
			};
		}/// namespace Bin
	}/// namespace Serialization
}/// namespace Stun

#undef _Word

