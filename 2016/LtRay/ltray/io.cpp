#ifdef USING_GLFW
#include <GLFW/glfw3.h>
#endif
#include <assert.h>
#include <string>
#include <cstring>
#include <fstream>
#include <thread> 
#include <iostream>
#include <sstream>
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
#ifdef USING_GLFW

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

#endif
}

ImageWriter::ImageWriter(std::string filename, int width, int height, bool grayscale)
:filename_(filename), width_(width), height_(height), grayscale_(grayscale)
{
	assert(width > 0 && height > 0);
	int N = width_ * height_;
	int size = grayscale_ ? N : N*3;
	image_buffer_.resize(size);
}

ImageWriter::~ImageWriter()
{
	
}

void ImageWriter::Write()
{
	ofstream ofs(filename_, ios::binary);
	ostringstream iss;
	string flag = grayscale_ ? "P5" : "P6";
	iss << flag << endl;
	iss << width_ << ' ' << height_ << endl;
	iss << 255 << endl;
	string header = iss.str();
	ofs.write(header.c_str(), header.size() * sizeof(char));
	int N = image_buffer_.size();
	vector<unsigned char> ubuf(N);
	for (int i = 0; i < N; i++) {
		int t = min(max(int(image_buffer_[i] * 255.f), 0), 255);
		ubuf[i] = unsigned char(t);
	}
	ofs.write((char*)ubuf.data(), ubuf.size()*sizeof(unsigned char));
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
