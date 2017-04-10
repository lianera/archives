#define _USE_MATH_DEFINES
#include <math.h>
#include "sphericalharmonics.h"

using namespace std;

const float PI = float(M_PI);

template class HarmonicIntegral<4>;
template class HarmonicIntegral<9>;
template class HarmonicIntegral<16>;


// reference: https://en.wikipedia.org/wiki/Table_of_spherical_harmonics

template<>
void HarmonicBasis<4>(float basis[4], float x, float y, float z, float r)
{
	basis[0] = 1.f / 2.f * sqrt(1.f / PI);
	basis[1] = sqrt(3.f / (4.f*PI))*y / r;
	basis[2] = sqrt(3.f / (4.f*PI))*z / r;
	basis[3] = sqrt(3.f / (4.f*PI))*x / r;
}


template<>
void HarmonicBasis<9>(float basis[9], float x, float y, float z, float r)
{
	HarmonicBasis<4>(basis, x, y, z, r);
	float r2 = r*r;
	basis[4] = 1.f / 2.f * sqrt(15.f / PI) * x * y / r2;
	basis[5] = 1.f / 2.f * sqrt(15.f / PI) * y * z / r2;
	basis[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x*x - y*y + 2 * z*z) / r2;
	basis[7] = 1.f / 2.f * sqrt(15.f / PI) * z * x / r2;
	basis[8] = 1.f / 4.f * sqrt(15.f / PI) * (x*x - y*y) / r2;
}

template<>
void HarmonicBasis<16>(float basis[16], float x, float y, float z, float r)
{
	HarmonicBasis<9>(basis, x, y, z, r);
	float x2 = x*x, y2 = y*y, z2 = z*z, r3 = r*r*r;
	basis[9] = 1.f / 4.f*sqrt(35.f / (2.f*PI))*(3 * x2 - y2)*y / r3;
	basis[10] = 1.f / 2.f*sqrt(105.f / PI)*x*y*z / r3;
	basis[11] = 1.f / 4.f*sqrt(21.f / (2.f*PI))*y*(4 * z2 - x2 - y2) / r3;
	basis[12] = 1.f / 4.f*sqrt(7.f / PI)*z*(2 * z2 - 3 * x2 - 3 * y2) / r3;
	basis[13] = 1.f / 4.f*sqrt(21.f / (2.f*PI))*x*(4 * z2 - x2 - y2) / r3;
	basis[14] = 1.f / 4.f*sqrt(105.f / PI)*(x2 - y2)*z / r3;
	basis[15] = 1.f / 4.f*sqrt(35.f / (2 * PI))*(x2 - 3 * y2)*x / r3;

}

template<int COEF_NUM>
HarmonicIntegral<COEF_NUM>::HarmonicIntegral()
{
	for (int i = 0; i < COEF_NUM; i++)
		coef[i] = {0,0,0};
}


template<int COEF_NUM>
void HarmonicIntegral<COEF_NUM>::operator()(XYZRGB pixel)
{
	float basis[COEF_NUM];
	float x = pixel.pos.x, y = pixel.pos.y, z = pixel.pos.z;
	float r = sqrt(x*x + y*y + z*z);
	HarmonicBasis<COEF_NUM>(basis, x, y, z, r);
	for (int i = 0; i < COEF_NUM; i++){
		coef[i].r += pixel.color.r*basis[i];
		coef[i].g += pixel.color.g*basis[i];
		coef[i].b += pixel.color.b*basis[i];
	}
	count++;
}

template<int COEF_NUM>
std::array<RGB, COEF_NUM> HarmonicIntegral<COEF_NUM>::getCoefficients()
{
	std::array<RGB, COEF_NUM> coefficients = coef;

	for (auto& c : coefficients){
		c.r = c.r * 4 * PI / count;
		c.g = c.g * 4 * PI / count;
		c.b = c.b * 4 * PI / count;
	}
	return coefficients;
}