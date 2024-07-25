#pragma once

// returns random number between -1.0 to 1.0
float Rand();
float URand();
float Rand(float min, float max);
float Rand(float max);
void RandSeed(unsigned int seed);
void RandDisk(float *in, const unsigned npoints, const float rad = 1.0f, const unsigned offset = 2);
