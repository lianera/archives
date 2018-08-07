#pragma once
#ifndef IO_H
#define IO_H

#include <thread>  
#include <queue>
#include <mutex>
#include <tuple>
#include <vector>
#include <memory>

class Canvas{
public:
	Canvas(int width, int height, bool grayscale = false);
	~Canvas();
	void setPixel(int x, int y, float r, float g, float b);

private:
	typedef std::tuple<int, int, float, float, float> ImagePixel;
	int width_, height_;
	bool grayscale_;

	std::vector<float> image_buffer_;
	std::thread* update_thread_;
	std::mutex task_mutex_;
	std::queue<ImagePixel> pixel_write_task_;

	void WindowProc();
	Canvas(const Canvas& out);	// prevent copy

};


class ImageWriter{
public:
	ImageWriter(std::string filename, int width, int height, bool grayscale = false);
	~ImageWriter();
	void setPixel(int x, int y, float r, float g, float b)
	{
		assert(x >= 0 && y >= 0 && x < width_ && y < height_);
		int p = (height_ - y - 1)*width_ + x;
		if (grayscale_){
			image_buffer_[p] = r;
		}
		else{
			image_buffer_[p * 3] = r;
			image_buffer_[p * 3 + 1] = g;
			image_buffer_[p * 3 + 2] = b;
		}
	}
	void Write();
private:
	std::string filename_;
	int width_, height_;
	bool grayscale_;
	std::vector<float> image_buffer_;
};


// output point cloud

class PLYWriter{

public:
	PLYWriter(std::string filename);
	~PLYWriter();
	void addPoint(float px, float py, float pz, float r, float g, float b);
	void Write();
private:
	typedef std::tuple<float, float, float, float, float, float> CloudPoint;

	std::queue<CloudPoint> pointcloud_;
	std::string filename_;
};

#endif