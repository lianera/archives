#ifndef CONVERT_H_
#define CONVERT_H_

#include <math.h>
#include <vector>
#include <array>
#include <functional>
#include <memory>
#include <fstream>
#include <iostream>
#include <string>
#include <FreeImage.h>

enum CUBEMAP_FACES{ POSX, NEGX, POSY, NEGY, POSZ, NEGZ};

struct XYZ{ float x, y, z; };
struct RGB{ float r, g, b; };
struct XYZRGB{ XYZ pos; RGB color; };

std::ostream& operator<<(std::ostream& os, RGB color);

class Cubemap{
	std::array<std::string, 6> image_files;
	int rows, cols;
public:
	Cubemap(std::array<std::string, 6> image_filenames);
	template<typename T>
	void Read(T& proc);
};

class WritePLY{
	std::shared_ptr<std::ofstream> plyfs;
public:
	WritePLY(std::string filename, int size);
	void operator()(XYZRGB pixel);
};

void convert_cube_uv_to_xyz(int index, float u, float v, float *x, float *y, float *z);
void convert_xyz_to_cube_uv(float x, float y, float z, int *index, float *u, float *v);

template<typename T>
void Cubemap::Read(T& proc)
{
	for (int k = 0; k < 6; k++){
		// load image
		FREE_IMAGE_FORMAT fif = FreeImage_GetFileType(image_files[k].c_str());
		FIBITMAP* img = FreeImage_Load(fif, image_files[k].c_str());
		if (!img)
			throw runtime_error("load " + image_files[k] + " failed");
		if (k == 0){
			cols = FreeImage_GetWidth(img);
			rows = FreeImage_GetHeight(img);
		}
		else{
			if (FreeImage_GetWidth(img) != cols || FreeImage_GetHeight(img) != rows)
				throw runtime_error("size of images are not match");
		}

		// read pixels
		float w = float(cols - 1), h = float(rows - 1);
		for (int j = 0; j < rows; j++){	// FreeImage scan from bottom to top
			for (int i = 0; i < cols; i++){
				RGBQUAD c;
				FreeImage_GetPixelColor(img, i, j, &c);
				RGB color = { float(c.rgbRed)/255.f, float(c.rgbGreen)/255.f, float(c.rgbBlue)/255.f };
				float u = float(i) / w;
				float v = float(j) / h;
				XYZ p;
				convert_cube_uv_to_xyz(k, u, v, &p.x, &p.y, &p.z);
				proc({ p, color });
			}
		}
		FreeImage_Unload(img);
	}
}


#endif