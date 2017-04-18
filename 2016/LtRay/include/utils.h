#pragma once
#ifndef UTILS_H
#define UTILS_H

#include <limits>
#include <string>

// very small number, usually used to check the equality between floats
const float EPSILON = 1e-5f;

const float PI = 3.14159265359f;

const float INF = std::numeric_limits<float>::infinity();


class Error{
	std::string info_;
public:
	Error(const std::string& info)
		:info_(info)
	{

	}
	std::string what()const
	{
		return info_;
	}
};

#endif