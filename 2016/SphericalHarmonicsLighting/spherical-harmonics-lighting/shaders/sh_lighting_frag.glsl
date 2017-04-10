#version 330

const int SH_NUM = 9;

const float PI = 3.1415926535897932384626433832795;

uniform vec3 coef[16];

in VS_OUT{
	vec3 normal;
}vs;

out vec4 color;

void main(void) {
	float basis[16];

	float x = vs.normal.x;
	float y = vs.normal.y;
	float z = vs.normal.z;
	float x2 = x*x;
	float y2 = y*y;
	float z2 = z*z;

	basis[0] = 1.f / 2.f * sqrt(1.f / PI);
	basis[1] = sqrt(3.f / (4.f*PI))*y;
	basis[2] = sqrt(3.f / (4.f*PI))*z;
	basis[3] = sqrt(3.f / (4.f*PI))*x;
	basis[4] = 1.f / 2.f * sqrt(15.f / PI) * x * y;
	basis[5] = 1.f / 2.f * sqrt(15.f / PI) * y * z;
	basis[6] = 1.f / 4.f * sqrt(5.f / PI) * (-x*x - y*y + 2 * z*z);
	basis[7] = 1.f / 2.f * sqrt(15.f / PI) * z * x;
	basis[8] = 1.f / 4.f * sqrt(15.f / PI) * (x*x - y*y);
	basis[9] = 1.f / 4.f*sqrt(35.f / (2.f*PI))*(3 * x2 - y2)*y;
	basis[10] = 1.f / 2.f*sqrt(105.f / PI)*x*y*z;
	basis[11] = 1.f / 4.f*sqrt(21.f / (2.f*PI))*y*(4 * z2 - x2 - y2);
	basis[12] = 1.f / 4.f*sqrt(7.f / PI)*z*(2 * z2 - 3 * x2 - 3 * y2);
	basis[13] = 1.f / 4.f*sqrt(21.f / (2.f*PI))*x*(4 * z2 - x2 - y2);
	basis[14] = 1.f / 4.f*sqrt(105.f / PI)*(x2 - y2)*z;
	basis[15] = 1.f / 4.f*sqrt(35.f / (2 * PI))*(x2 - 3 * y2)*x;

	vec3 c = vec3(0,0,0);
	for (int i = 0; i < SH_NUM; i++)
		c += coef[i] * basis[i];
	color = vec4(c, 1);
}