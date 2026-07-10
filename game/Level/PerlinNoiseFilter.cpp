#include "Level/PerlinNoiseFilter.h"

#include <random>

#define FUZZINESS 16

/* code from: https://github.com/thecodeofnotch/rd-20090515/blob/master/src/main/java/com/mojang/rubydung/level/PerlinNoiseFilter.java */

static std::mt19937 rng(std::random_device{}());

static int randRange(int min, int max) 
{
	return std::uniform_int_distribution<int>(min, max)(rng);
}

PerlinNoiseFilter::PerlinNoiseFilter(int octave)
	: m_Octave(octave)
{

}

PerlinNoiseFilter::~PerlinNoiseFilter()
{

}

int *PerlinNoiseFilter::read(int width, int height, int *out)
{
	int *table = (int*)malloc(width * height *  sizeof(int));
	if (!table)
		return NULL;

	/* create first table */
	for (int step = width >> m_Octave, y = 0; y < height; y += step) {
		for (int x = 0; x < width; x += step) {
			table[x + y * width] = randRange(-128, 128) * FUZZINESS;
		}
	}

	for (int step = width >> m_Octave; step > 1; step /= 2) {
		int max = 256 * (step << m_Octave);
		int halfstep = step / 2;

		/* first mutation */
		for (int y = 0; y < height; y += step) {
			for (int x = 0; x < width; x += step) {
				int v = table[x % width + y % height * width];

				int SX = table[(x + step) % width + y % height * width];
				int SY = table[x % width + (y + step) % height * width];
				int SXY = table[(x + step) % width + (y + step) % height * width];

				int mutatedValue = (v + SY + SX + SXY) / 4 + randRange(-max, max);

				table[x + halfstep + (y + halfstep) * width] = mutatedValue;
			}
		}

		for (int y = 0; y < height; y += step) {
			for (int x = 0; x < width; x += step) {
				int v = table[x + y * width];

				int SX = table[(x + step) % width + y * width];
				int SY = table[x + ((y + step) % height) * width];

				int hSX = table[((x + halfstep) & (width - 1)) + ((y + halfstep - step) & (height - 1)) * width];
				int hSY = table[((x + halfstep - step) & (width - 1)) + ((y + halfstep) & (height - 1)) * width];

				int hSv = table[(x + halfstep) % width + (y + halfstep) % height * width];

				int mutatedValueX = (v + SX + hSv + hSX) / 4 + randRange(-max, max);
				int mutatedValueY = (v + SY + hSv + hSY) / 4 + randRange(-max, max);

				table[x + halfstep + y * width] = mutatedValueX;
				table[x + (y + halfstep) * width] = mutatedValueY;
			}
		}
	}
	
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			out[x + y * width] = table[x % width + y % height * width] / 512 + 128;
		}
	}

	free(table);

	return out;
}
