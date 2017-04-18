#include <assert.h>
#include <GLFW/glfw3.h>
#include <FreeImage.h>
#include <string>
#include <cstring>
#include <fstream>
#include <thread> 
#include <iostream>
#include <mutex>
#include "io.h"

using namespace std;

Canvas::Canvas(int width, int height, bool grayscale)
:width_(width), height_(height), grayscale_(grayscale)
{
	assert(width > 0);
	assert(height > 0);

	// TODO: for unknow reason, gl cannot display intensity, so using rgb temporarily, 
	// thus this version is just for preview, please output to file for accurate depth image
	image_buffer_.resize(width*height*3);
	update_thread_ = new std::thread(std::bind(&Canvas::WindowProc, this));

}

Canvas::~Canvas()
{
	update_thread_->join();
	delete update_thread_;
}

void Canvas::setPixel(int x, int y, float r, float g, float b)
{
	task_mutex_.lock();
	pixel_write_task_.push(ImagePixel(x, y, r, g, b));
	task_mutex_.unlock();
}

void Canvas::WindowProc()
{
	GLFWwindow* window;

	// init
	if (!glfwInit())
		throw runtime_error("Init glfw failed");

	window = glfwCreateWindow(width_, height_, "LtRay", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		throw runtime_error("Creat window failed");
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	glfwSwapInterval(1);

	while (!glfwWindowShouldClose(window)){

		// Render here
		glClear(GL_COLOR_BUFFER_BIT);

		// write all pixels in task queue
		while (!pixel_write_task_.empty()){

			task_mutex_.lock();
			ImagePixel pixel = pixel_write_task_.front();
			pixel_write_task_.pop();
			task_mutex_.unlock();

			int x, y;
			float r, g, b;
			tie(x, y, r, g, b) = pixel;
			
			int scr_point = y * width_ + x;
			if (grayscale_){
				float scale = 1.0f / 65535.0f;
				r *= scale;
				g *= scale;
				b *= scale;
			}
			image_buffer_[scr_point * 3] = r;
			image_buffer_[scr_point * 3 + 1] = g;
			image_buffer_[scr_point * 3 + 2] = b;

		}

		glDrawPixels(width_, height_, GL_RGB, GL_FLOAT, &image_buffer_[0]);

		// Swap front and back buffers 
		glfwSwapBuffers(window);

		// Poll for and process events
		glfwPollEvents();

	}
	glfwTerminate();
}

int ImageWriter::count = 0;

ImageWriter::ImageWriter(std::string filename, int width, int height, bool grayscale)
:filename_(filename), width_(width), height_(height), grayscale_(grayscale)
{
	assert(width > 0 && height > 0);
	if (count == 0)
		FreeImage_Initialise();
	count++;
	int size = grayscale_ ? width_ * height_ : width_ * height_ * 3;
	image_buffer_.resize(size);

}

ImageWriter::~ImageWriter()
{
	count--;
	if (count == 0)
		FreeImage_DeInitialise();
}

void ImageWriter::Write()
{
	FIBITMAP* bitmap = grayscale_ ?
		FreeImage_AllocateT(FIT_UINT16, width_, height_)
		: FreeImage_Allocate(width_, height_, 24);
	// get file extension
	bool supported_ext = true;
	FREE_IMAGE_FORMAT format = FIF_PNG;
	string::size_type p = filename_.find_last_of('.');
	if (p != string::npos){
		string ext = filename_.substr(p, filename_.size() - p);
		for (char& c : ext)
			c = toupper(c);
		if (ext == ".PNG")
			format = FIF_PNG;
		else if (ext == ".BMP")
			format = FIF_BMP;
		else if (ext == ".PBM")
			format = FIF_PBM;
		else if (ext == ".PGM")
			format = FIF_PGM;
		else if (ext == ".PPM")
			format = FIF_PPM;
		else if (ext == ".JPG" || ext == ".JPEG")
			format = FIF_JPEG;
		else
			supported_ext = false;
	}
	else{
		supported_ext = false;
	}
	if (!supported_ext){
		format = FIF_PNG;
		cout << "unsupported image format, using .png as replacement" << endl;
	}
	if (grayscale_){
		for (int j = 0; j < height_; j++) {
			BYTE* scanline = FreeImage_GetScanLine(bitmap, j);
			unsigned short* bits = (unsigned short*)scanline;
			for (int i = 0; i < width_; i++) {
				*bits = (unsigned short)max(min(int(
					image_buffer_[j * width_ + i]), 65535), 0);
				bits++;
			}
		}
	}
	else{
		for (int j = 0; j < height_; j++) {
			for (int i = 0; i < width_; i++) {
				int p = (j * width_ + i) * 3;
				RGBQUAD c;
				c.rgbRed = max(min(int(image_buffer_[p] * 255), 255), 0);
				c.rgbGreen = max(min(int(image_buffer_[p+1] * 255), 255), 0);
				c.rgbBlue = max(min(int(image_buffer_[p+2] * 255), 255), 0);
				FreeImage_SetPixelColor(bitmap, i, j, &c);
			}
		}
	}

	FreeImage_Save(format, bitmap, filename_.c_str());
}


PLYWriter::PLYWriter(string filename)
:filename_(filename)
{

}

PLYWriter::~PLYWriter()
{
}

void PLYWriter::addPoint(float px, float py, float pz, float r, float g, float b)
{
	// get rid of invalid points
	if (isfinite(px) && isfinite(r) &&
		isfinite(py) && isfinite(g) &&
		isfinite(pz) && isfinite(b))
	{
		pointcloud_.push(CloudPoint(px,py,pz,r,g,b));
	}
}

void PLYWriter::Write()
{
	ofstream ofs_(filename_, ios::binary);
	if (!ofs_)
		throw runtime_error("Cannot write to " + filename_);
	// write header
	ofs_ << "ply" << endl;
	ofs_ << "format binary_little_endian 1.0" << endl;
	ofs_ << "element vertex " << pointcloud_.size() << endl;
	ofs_ << "property float x" << endl;
	ofs_ << "property float y" << endl;
	ofs_ << "property float z" << endl;
	ofs_ << "property uchar red" << endl;
	ofs_ << "property uchar green" << endl;
	ofs_ << "property uchar blue" << endl;
	ofs_ << "end_header" << endl;

	int bufsize = (sizeof(float)* 3 + sizeof(char)* 3)*pointcloud_.size();
	vector<char> buf(bufsize);
	char* p = &buf[0];
	while (!pointcloud_.empty()) {
		float px, py, pz, r, g, b;
		tie(px, py, pz, r, g, b) = pointcloud_.front();
		pointcloud_.pop();
		float* pf = (float*)p;
		*pf++ = px;
		*pf++ = py;
		*pf++ = pz;
		p = (char*)pf;
		*p++ = (char)std::min(int(r * 255), 255);
		*p++ = (char)std::min(int(g * 255), 255);
		*p++ = (char)std::min(int(b * 255), 255);
	}
	ofs_.write(&buf[0], buf.size());
}
