#pragma once

#include "common.h"

/* Random number generator from: https://github.com/openjdk/jdk/blob/master/src/java.base/share/classes/java/util/Random.java */

namespace MC 
{
	class MC_API Random 
	{
	public:
		u64t m_Seed;
		static std::atomic<u64t> m_seedUniquifier;
	public:
		Random();
		Random(u64t seed);
		~Random();
	public:
		void SetSeed(u64t seed);
	private:
		static u64t SeedUniquifier();
		static u64t InitialScramble(u64t seed);
	protected:
		int Next(int bits);
	public:
		int NextInt();
		int NextInt(int bound);
		u64t NextLong();
		bool NextBool();
		float NextFloat();
		double NextDouble();
	};
}