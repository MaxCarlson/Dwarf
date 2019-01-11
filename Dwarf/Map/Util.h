#pragma once
#include "../Coordinates.h"

#include <vector>
#include <cstdint>
#include <limits>
#include <type_traits>
#include <memory>

namespace Util
{
	template<std::uint64_t K>
	class Bitset
	{
	public:
		decltype(K) k_ = K;

		typename std::conditional < K <= UINT8_MAX,
			std::uint8_t,
			typename std::conditional < K <= UINT16_MAX,
			std::uint16_t,
			typename std::conditional < K <= UINT32_MAX,
			std::uint32_t,
			std::uint64_t>::type>::type>::type bits = 0;

		void set(const uint16_t flag)
		{
			bits |= flag;
		}

		void reset(const uint16_t flag)
		{
			bits &= ~flag;
		}

		void clear()
		{
			bits = 0;
		}

		bool test(const uint16_t flag) const
		{
			return bits & flag;
		}

		template<class Archive>
		void serialize(Archive & archive)
		{
			archive(k_);
			archive(bits);
		}
	};
	enum 
	{
		BITSET_8  = 0,
		BITSET_16 = std::numeric_limits<std::uint16_t>::max(),
		BITSET_32 = std::numeric_limits<std::uint32_t>::max(), // These are -1, why?
		BITSET_64 = std::numeric_limits<std::uint64_t>::max()
	};
}
