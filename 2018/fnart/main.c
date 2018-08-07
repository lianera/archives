#define _USE_MATH_DEFINES
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#define EPSILON 0.01f

#define uchar unsigned char

typedef struct {
	double r, g, b;
}Color;

double Mandelbrot(double cre, double cim)
{
	const int M = 255;
	const double esc = 2.0;
	double zre = 0.0, zim = 0.0;
	for (int i = 0; i < M; i++) {
		double fre = zre * zre - zim * zim + cre;
		double fim = 2 * zre*zim + cim;
		zre = fre;
		zim = fim;
		double magsq = zre * zre + zim * zim;
		if (magsq > esc*esc) {
			// smooth
			double smi = i - log(0.5*log(magsq)) / log(2.0);
			return smi / M;
		}
	}
	return -INFINITY;
}

inline void Color2UChar(Color c, uchar rgb[3]);
void Color2UChar(Color c, uchar rgb[3])
{
	int r = (int)(c.r*255.0);
	int g = (int)(c.g*255.0);
	int b = (int)(c.b*255.0);
	rgb[0] = (uchar)(r<0 ? 0 : r>255 ? 255 : r);
	rgb[1] = (uchar)(g<0 ? 0 : g>255 ? 255 : g);
	rgb[2] = (uchar)(b<0 ? 0 : b>255 ? 255 : b);
}

void PPMWriter(const char* filename, int w, int h, uchar* pixels)
{
	FILE* fp = fopen("out.ppm", "wb");
	if (!fp)
		perror("open file failed");
	char header[64];
	sprintf(header, "P6\n%d %d\n255\n", w, h);
	fwrite(header, sizeof(uchar), strlen(header), fp);
	fwrite(pixels, sizeof(uchar), w*h * 3, fp);
	fclose(fp);
}

void DrawMandelbrot(int width, int height, double x0, double y0, double range)
{
	double yrange = range * height / width;
	double xmin = x0 - range, xmax = x0 + range;
	double ymin = y0 - yrange, ymax = y0 + yrange;
	uchar* pixels = malloc(width*height * 3);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			double x = (double)j / (width - 1)*(xmax - xmin) + xmin;
			double y = (double)(height - i - 1) / (height - 1)*(ymax - ymin) + ymin;
			double val = Mandelbrot(x, y);			
			// tone mapping
			double t = 0.5*(1.0 - cos(val*5.0*M_PI));			
			double u = t+0.5;
			double v = 2.0 - 2.0*t;
			double w = 0.5*(1.0 - cos(t*3.0*M_PI));
			Color rgb = {
				t < 0.5 ? u : 1,
				w,
				t < 0.5 ? 1 : v
			};
			Color black = {0,0,0};
			int idx = i * width + j;
			Color c = isfinite(t) ? rgb : black;
			Color2UChar(c , pixels + idx * 3);
		}
	}
	PPMWriter("out.ppm", width, height, pixels);
	free(pixels);
}

int main(int argc, char* argv[])
{
	int width = atoi(argv[1]);
	int height = atoi(argv[2]);
	double x0 = atof(argv[3]);
	double y0 = atof(argv[4]);
	double range = atof(argv[5]);
	DrawMandelbrot(width, height, x0, y0, range);
	return 0;
}

