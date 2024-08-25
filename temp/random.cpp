

#include "pch.h"

#include "random.h"

namespace Stun {
	namespace Utils {

		std::random_device Random::rd;
		std::mt19937 Random::gen;

	}/// Utils
}/// Stun
