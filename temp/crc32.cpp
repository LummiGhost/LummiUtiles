
#include "pch.h"

#include "crc32.hpp"


const std::array<uint32_t, 256> Stun::Utils::CRC32::crc32_table = CRC32::generate_crc32_table(std::make_index_sequence<256>{});
