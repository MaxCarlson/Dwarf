#pragma once
#include "../Coordinates.h"

#include <vector>
#include <bitset>
#include <cstdint>
#include <limits>
#include <type_traits>

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
			std::uint64_t>::type>::type>::type bits;

		void set(const uint16_t flag)
		{
			bits |= flag;
		}

		void reset(const uint16_t flag)
		{
			bits &= ~flag;
		}

		bool test(const uint16_t flag) const
		{
			return bits & flag;
		}
	};
	enum
	{
		BITSET_8 = 0,
		BITSET_16 = std::numeric_limits<std::int16_t>::max(),
		BITSET_32 = std::numeric_limits<std::int32_t>::max(),
		BITSET_64 = std::numeric_limits<std::int64_t>::max()
	};
}



class Region
{
public:
	Region(int width, int height, int depth);

	int width, height, depth;

	std::vector<char> explored;
	std::vector<char> solid;

	std::vector<uint8_t> tileType;
	std::vector<uint8_t> material;
	std::vector<uint16_t> health;

	std::vector<Util::Bitset<Util::BITSET_16>> tileFlags;
};

extern Region region;

enum Flag
{

};


inline bool canWalk(Coordinates co);

inline bool flag(Coordinates co, Flag f);

inline void setFlag(Coordinates co, Flag f);

inline void resetFlag(Coordinates co, Flag f);

