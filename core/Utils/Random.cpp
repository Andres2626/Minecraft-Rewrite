#include "Utils/Random.h"

namespace MC 
{
	std::atomic<u64t> Random::m_seedUniquifier = { 8682522807148012ULL };

	Random::Random()
		: Random(Random::SeedUniquifier() ^ std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count())
	{
	}

	Random::Random(u64t seed)
		: m_Seed(InitialScramble(seed))
	{

	}

	Random::~Random()
	{
	}

	u64t Random::SeedUniquifier()
	{
		for (;;) {
			u64t current = Random::m_seedUniquifier.load();
			u64t next = current * 1181783497276652981ULL;
			if (m_seedUniquifier.compare_exchange_weak(current, next))
				return next;
		}
	}

	void Random::SetSeed(u64t seed)
	{
		m_Seed = InitialScramble(seed);
	}

	u64t Random::InitialScramble(u64t seed)
	{
		return (seed ^ 0x5DEECE66DULL) & ((1ULL << 48) - 1);
	}

	int Random::NextInt()
	{
		return Next(32);
	}

	int Random::NextInt(int bound)
	{
		if (!bound)
			assert(bound > 0);

		int r = Next(31);
		int m = bound - 1;
		if ((bound & m) == 0)
			r = (int)((bound * (u64t)r) >> 31);
		else
			for (int u = r; u - (r = u % bound) + m < 0; u = Next(31));

		return r;
	}

	u64t Random::NextLong()
	{
		return ((u64t)(Next(32)) << 32) + Next(32);
	}

	bool Random::NextBool()
	{
		return Next(1) != 0;
	}

	int Random::Next(int bits)
	{
		m_Seed = (m_Seed * 0x5DEECE66DULL + 0xBULL)& ((1ULL << 48) - 1);
		return (int)(m_Seed >> (48 - bits));
	}

	float Random::NextFloat()
	{
		return Next(24) * 0x1.0p-24f;
	}

	double Random::NextDouble()
	{
		return (((u64t)(Next(26)) << 27) + Next(27)) * 0x1.0p-53;
	}
}


