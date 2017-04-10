#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <array>
#include <functional>
#include "convert.h"

using namespace std;

void convert_cube_uv_to_xyz(int index, float u, float v, float *x, float *y, float *z)
{
	// convert range 0 to 1 to -1 to 1
	float uc = 2.0f * u - 1.0f;
	float vc = 2.0f * v - 1.0f;
	switch (index)
	{
	case 0: *x = 1.0f; *y = vc; *z = -uc; break;	// POSITIVE X
	case 1: *x = -1.0f; *y = vc; *z = uc; break;	// NEGATIVE X
	case 2: *x = uc; *y = 1.0f; *z = -vc; break;	// POSITIVE Y
	case 3: *x = uc; *y = -1.0f; *z = vc; break;	// NEGATIVE Y
	case 4: *x = uc; *y = vc; *z = 1.0f; break;	// POSITIVE Z
	case 5: *x = -uc; *y = vc; *z = -1.0f; break;	// NEGATIVE Z
	}
}

void convert_xyz_to_cube_uv(float x, float y, float z, int *index, float *u, float *v)
{
	float absX = fabs(x);
	float absY = fabs(y);
	float absZ = fabs(z);

	int isXPositive = x > 0 ? 1 : 0;
	int isYPositive = y > 0 ? 1 : 0;
	int isZPositive = z > 0 ? 1 : 0;

	float maxAxis, uc, vc;

	// POSITIVE X
	if (isXPositive && absX >= absY && absX >= absZ) {
		// u (0 to 1) goes from +z to -z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = -z;
		vc = y;
		*index = 0;
	}
	// NEGATIVE X
	if (!isXPositive && absX >= absY && absX >= absZ) {
		// u (0 to 1) goes from -z to +z
		// v (0 to 1) goes from -y to +y
		maxAxis = absX;
		uc = z;
		vc = y;
		*index = 1;
	}
	// POSITIVE Y
	if (isYPositive && absY >= absX && absY >= absZ) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from +z to -z
		maxAxis = absY;
		uc = x;
		vc = -z;
		*index = 2;
	}
	// NEGATIVE Y
	if (!isYPositive && absY >= absX && absY >= absZ) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -z to +z
		maxAxis = absY;
		uc = x;
		vc = z;
		*index = 3;
	}
	// POSITIVE Z
	if (isZPositive && absZ >= absX && absZ >= absY) {
		// u (0 to 1) goes from -x to +x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = x;
		vc = y;
		*index = 4;
	}
	// NEGATIVE Z
	if (!isZPositive && absZ >= absX && absZ >= absY) {
		// u (0 to 1) goes from +x to -x
		// v (0 to 1) goes from -y to +y
		maxAxis = absZ;
		uc = -x;
		vc = y;
		*index = 5;
	}

	// Convert range from -1 to 1 to 0 to 1
	*u = 0.5f * (uc / maxAxis + 1.0f);
	*v = 0.5f * (vc / maxAxis + 1.0f);
}

Cubemap::Cubemap(std::array<std::string, 6> image_filenames)
	:image_files(image_filenames)
{
}

WritePLY::WritePLY(string filename, int size)
{
	plyfs.reset(new ofstream(filename, ios::binary));
	ofstream& ofs = *plyfs;
	if (!ofs)
		throw runtime_error("Cannot open " + filename);
	// write header
	ofs << "ply" << endl;
	ofs << "format binary_little_endian 1.0" << endl;
	ofs << "element vertex " << size << endl;
	ofs << "property float x" << endl;
	ofs << "property float y" << endl;
	ofs << "property float z" << endl;
	ofs << "property uchar red" << endl;
	ofs << "property uchar green" << endl;
	ofs << "property uchar blue" << endl;
	ofs << "end_header" << endl;

}

namespace{

inline unsigned char uchar_color(float c)
{
	return (unsigned char)max(min(int(c * 255), 255), 0);
}
};

void WritePLY::operator()(XYZRGB pixel)
{
	char buf[sizeof(float)*3 + sizeof(unsigned char)*3];
	float *p = (float*)buf;
	p[0] = pixel.pos.x;
	p[1] = pixel.pos.y;
	p[2] = pixel.pos.z;
	unsigned char *c = (unsigned char*)(buf + sizeof(float)* 3);
	c[0] = uchar_color(pixel.color.r);
	c[1] = uchar_color(pixel.color.g);
	c[2] = uchar_color(pixel.color.b);
	plyfs->write(buf, sizeof(buf));
}


std::ostream& operator<<(std::ostream& os, RGB color)
{
	os << color.r << '\t' << color.g << '\t' << color.b;
	return os;
}