#ifndef SPHERICAL_HARMONICS_H_
#define SPHERICAL_HARMONICS_H_

#include <array>
#include "convert.h"

// normalize first
template<int COEF_NUM>
void HarmonicBasis(float basis[4], float x, float y, float z, float r = 1.f);

template <int COEF_NUM>
class HarmonicIntegral{
	int count = 0;
	std::array<RGB, COEF_NUM> coef;
public:
	HarmonicIntegral();
	void operator()(XYZRGB pixel);
	std::array<RGB, COEF_NUM> getCoefficients();
};

#endif