#pragma once


#include <random>

namespace Stun {
	namespace Utils {
		class Random {
			static std::random_device rd;
			static std::mt19937 gen;
		public:
			static void Reset() {
				gen.seed(rd());
			}
			static void Reset(unsigned int seed) {
				gen.seed(seed);
			}

			template<typename T>
			static T Next() {
				return std::uniform_int_distribution<T>()(gen);
			}
			template<typename T>
			static T Next(T min, T max) {
				return std::uniform_int_distribution<T>(min, max)(gen);
			}

			static int Fill(uint32_t* buffer, size_t count) {
				for (size_t i = 0; i < count; i++) {
					buffer[i] = Next<uint32_t>();
				}
				return (int)count;
			}
		};

		//std::random_device Random::rd;
		//std::mt19937 Random::gen;
	}
}