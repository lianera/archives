#pragma once
#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <cmath>
#include <iostream>
#include <vector>
#include "utils.h"

namespace ltray{

/** vector, x y z w(0)
*/
struct Vector{
	float x, y, z, w = 0;
	// default (0,0,0)
	Vector()
		:x(0), y(0), z(0)
	{}

	Vector(float x_, float y_, float z_)
		:x(x_), y(y_), z(z_)
	{}

	Vector(const std::vector<float>& vec)
		:x(vec[0]), y(vec[1]), z(vec[2])
	{}

	bool IsZero() const
	{
		return std::abs(x) <= EPSILON && std::abs(y) <= EPSILON && std::abs(z) <= EPSILON;
	}

	float& operator[](int i)
	{
		return (&x)[i];
	}
	const float& operator[](int i) const
	{
		return (&x)[i];
	}
	bool operator==(const Vector& other)const
	{
		return std::abs(x - other.x) <= EPSILON
			&& std::abs(y - other.y) <= EPSILON
			&& std::abs(z - other.z) <= EPSILON;
	}


	Vector operator-() const
	{
		return Vector(-x, -y, -z);
	}

	Vector operator+(const Vector& a) const
	{
		return Vector(x + a.x, y + a.y, z + a.z);
	}

	Vector operator-(const Vector& a) const
	{
		return Vector(x - a.x, y - a.y, z - a.z);
	}

	// dot product
	float Dot(const Vector& a) const
	{
		return x * a.x + y * a.y + z * a.z;
	}

	Vector operator/(const Vector& a) const
	{
		return Vector(x / a.x, y / a.y, z / a.z);
	}


	Vector& operator+=(const Vector& a)
	{
		x += a.x;
		y += a.y;
		z += a.z;
		return *this;
	}

	bool IsNormalized()const
	{
		return std::abs(SqSum() - 1) < EPSILON;
	}

	// return a normalized vector
	Vector Normalized() const
	{
		float inv_length = 1.0f / sqrt(x * x + y * y + z * z);
		return Vector(x * inv_length, y * inv_length, z * inv_length);
	}

	// Hadamard product
	Vector operator*(const Vector& a) const
	{
		return Vector(x * a.x, y * a.y, z * a.z);
	}
	// Cross product
	Vector Cross(const Vector& a) const
	{
		return Vector(y * a.z - z * a.y,
			z * a.x - x * a.z,
			x * a.y - y * a.x);
	}

	// 1-norm
	float Norm1()const
	{
		return std::abs(x) + std::abs(y) + std::abs(z);
	}
	// squared sum
	float SqSum()const
	{
		return x*x + y*y + z*z;
	}
	float InvSqNorm2()const
	{
		return 1.0f / SqSum();
	}
	// 2-norm(distance)
	float Norm2()const
	{
		return std::sqrt(SqSum());
	}

};



/** point, x y z w(1)
*/
struct Point {
	// NOTE: The w member of Point should be always 1
	float x, y, z, w = 1;

	Point()
		:x(0), y(0), z(0)
	{}

	Point(float x_, float y_, float z_)
		:x(x_), y(y_), z(z_)
	{}

	Point(const std::vector<float>& vec)
		:x(vec[0]), y(vec[1]), z(vec[2])
	{

	}

	float& operator[](int i)
	{
		return (&x)[i];
	}
	const float& operator[](int i) const
	{
		return (&x)[i];
	}

	bool operator==(const Point& p)const
	{
		return std::abs(x - p.x) <= EPSILON
			&& std::abs(y - p.y) <= EPSILON
			&& std::abs(z - p.z) <= EPSILON;
	}

	Point& operator+=(const Vector& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	Point& operator-=(const Vector& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}

	operator Vector() const
	{
		return Vector(x, y, z);
	}

};

inline Vector operator*(float a, const Vector& v)
{
	return Vector(a * v.x, a * v.y, a * v.z);
}

inline Vector operator*(const Vector& v, float a)
{
	return Vector(a * v.x, a * v.y, a * v.z);
}

inline Point operator+(const Point& p, const Vector& v)
{
	return Point(p.x + v.x, p.y + v.y, p.z + v.z);
}

inline Point operator+(const Vector& v, const Point& p)
{
	return Point(p.x + v.x, p.y + v.y, p.z + v.z);
}

inline Point operator-(const Point& p, const Vector& v)
{
	return Point(p.x - v.x, p.y - v.y, p.z - v.z);
}

inline Point operator-(const Vector& v, const Point& p)
{
	return Point(p.x - v.x, p.y - v.y, p.z - v.z);
}

inline Vector operator-(const Point& p1, const Point& p2)
{
	return Vector(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z);
}




/** 4x4 matrix
*/
class Matrix{
public:
	Matrix()
		:a11_(1), a12_(0), a13_(0), a14_(0),
		a21_(0), a22_(1), a23_(0), a24_(0),
		a31_(0), a32_(0), a33_(1), a34_(0),
		a41_(0), a42_(0), a43_(0), a44_(1)
	{}
	Matrix(float a11, float a12, float a13, float a14,
		float a21, float a22, float a23, float a24,
		float a31, float a32, float a33, float a34,
		float a41, float a42, float a43, float a44)
		:a11_(a11), a12_(a12), a13_(a13), a14_(a14),
		a21_(a21), a22_(a22), a23_(a23), a24_(a24),
		a31_(a31), a32_(a32), a33_(a33), a34_(a34),
		a41_(a41), a42_(a42), a43_(a43), a44_(a44)
	{}
	float& operator[](int i)
	{
		return (&a11_)[i];
	}
	const float& operator[](int i)const
	{
		return (&a11_)[i];
	}

	float& operator()(int i, int j)
	{
		return (&a11_)[4 * i + j];
	}

	const float& operator()(int i, int j)const
	{
		return (&a11_)[4 * i + j];
	}

	const float& at(int i, int j)const
	{
		return (&a11_)[4 * i + j];
	}



	friend bool operator==(const Matrix& m1, const Matrix& m2);
	friend Matrix operator+(const Matrix& m1, const Matrix& m2);
	friend Matrix operator-(const Matrix& m1, const Matrix& m2);
	friend Matrix operator*(const Matrix& m1, const Matrix& m2);


	Matrix& operator*=(const Matrix& m)
	{
		(*this) = m * (*this);
		return *this;
	}

	Point operator*(const Point& p)const
	{
		Point r(0, 0, 0);
		for (int i = 0; i < 4; i++) {
			r[i] = at(i, 0)*p[0] + at(i, 1)*p[1] + at(i, 2)*p[2] + at(i, 3)*p[3];
		}
		return r;
	}
	Vector operator*(const Vector& v)const
	{
		Vector r(0, 0, 0);
		for (int i = 0; i < 4; i++) {
			r[i] = at(i, 0)*v[0] + at(i, 1)*v[1] + at(i, 2)*v[2] + at(i, 3)*v[3];
		}
		return r;
	}

	friend Matrix operator*(const Matrix& m, float val);
	friend Matrix operator*(float val, const Matrix& m);


	// Return a Transposed Matrix
	Matrix T()const
	{
		return Transpose(*this);
	}

	static Matrix Transpose(const Matrix& m)
	{
		Matrix r;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				r(i, j) = m(j, i);
			}
		}
		return r;
	}

	//--- frequently used matrix ---

	static const Matrix& ZeroMatrix()
	{
		static const Matrix m(
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 0, 0, 0
			);
		return m;
	}

	static const Matrix& IdentityMatrix()
	{
		static const Matrix m(
			1, 0, 0, 0,
			0, 1, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1
			);
		return m;
	}

	static Matrix DiagMatrix(float a)
	{
		return Matrix(
			a, 0, 0, 0,
			0, a, 0, 0,
			0, 0, a, 0,
			0, 0, 0, a
			);
	}

	static Matrix ScaleMatrix(float sx, float sy, float sz)
	{
		return Matrix(
			sx, 0, 0, 0,
			0, sy, 0, 0,
			0, 0, sz, 0,
			0, 0, 0, 1);
	}

	static Matrix TranslateMatrix(float tx, float ty, float tz)
	{
		return Matrix(
			1, 0, 0, tx,
			0, 1, 0, ty,
			0, 0, 1, tz,
			0, 0, 0, 1);
	}

	static Matrix RotXMatrix(float phi)
	{
		float c = cos(phi);
		float s = sin(phi);
		return Matrix(
			1, 0, 0, 0,
			0, c, -s, 0,
			0, s, c, 0,
			0, 0, 0, 1);
	}

	static Matrix RotYMatrix(float phi)
	{
		float c = cos(phi);
		float s = sin(phi);
		return Matrix(
			c, 0, s, 0,
			0, 1, 0, 0,
			-s, 0, c, 0,
			0, 0, 0, 1);
	}

	static Matrix RotZMatrix(float phi)
	{
		float c = cos(phi);
		float s = sin(phi);
		return Matrix(
			c, -s, 0, 0,
			s, c, 0, 0,
			0, 0, 1, 0,
			0, 0, 0, 1);
	}
private:
	float	a11_, a12_, a13_, a14_,
		a21_, a22_, a23_, a24_,
		a31_, a32_, a33_, a34_,
		a41_, a42_, a43_, a44_;
};

// --- Matrix operators ---
inline bool operator==(const Matrix& m1, const Matrix& m2)
{
	for (int i = 0; i < 16; i++) {
		if (m1[i] != m2[i])
			return false;
	}
	return true;
}

inline Matrix operator-(const Matrix& m)
{
	Matrix r;
	for (int i = 0; i < 16; i++)
		r[i] = -m[i];
	return r;
}

inline Matrix operator+(const Matrix& m1, const Matrix& m2)
{
	Matrix r;
	for (int i = 0; i < 16; i++)
		r[i] = m1[i] + m2[i];
	return r;
}

inline Matrix operator-(const Matrix& m1, const Matrix& m2)
{
	Matrix r;
	for (int i = 0; i < 16; i++)
		r[i] = m1[i] - m2[i];
	return r;
}

inline Matrix operator*(const Matrix& m1, const Matrix& m2)
{
	Matrix r;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			r(i, j) = m1(i, 0)*m2(0, j) +
				m1(i, 1)*m2(1, j) +
				m1(i, 2)*m2(2, j) +
				m1(i, 3)*m2(3, j);
		}
	}
	return r;
}

inline Matrix operator*(const Matrix& m, float val)
{
	Matrix r;
	for (int i = 0; i < 16; i++)
		r[i] = m[i] * val;
	return r;
}

inline Matrix operator*(float val, const Matrix& m)
{
	return m * val;
}



/** Non-gap interval on Real Field (i.e. range)
*/
struct Interval{
	float lb;	// lower bound
	float ub;	// uppper bound

	// Null set by default
	Interval() :lb(NAN), ub(NAN){}
	Interval(float lower_bound, float upper_bound)
	{
		if (lower_bound > upper_bound){
			lb= NAN;
			ub= NAN;
		}
		else{
			lb = lower_bound;
			ub = upper_bound;
		}
	}
	
	//--- predefined intervals ---	
	static Interval NullSet()
	{
		return Interval(NAN, NAN);
	}

	static Interval NonNegtive()
	{
		return Interval(0.0f, INF);
	}

	static Interval Real()
	{
		return Interval(-INF, INF);
	}

	float Length()const
	{
		return IsNullSet() ? 0.0f : ub- lb;
	}
	
	// get the max range of two intervals
	Interval Range(const Interval& obj)const
	{
		float min = lb<= obj.lb? lb: obj.lb;
		float max = ub>= obj.ub? ub: obj.ub;
		return Interval(min, max);
	}

	Interval Intersect(const Interval& obj)const
	{
		if (obj.ub< lb|| ub< obj.lb)
			return NullSet();
		else if (obj.lb<= lb){
			if (obj.ub<= ub){ return Interval(lb, obj.ub); }
			else if (ub< obj.ub){ return Interval(lb, ub); }
		}
		else if (lb< obj.lb){
			if (obj.ub<= ub){ return Interval(obj.lb, obj.ub); }
			else if (ub< obj.ub){ return Interval(obj.lb, ub); }
		}
		return NullSet();
	}

	bool IsNullSet()const
	{
		return std::isnan(lb) || std::isnan(ub);
	}
	
	bool operator==(const Interval& obj)const
	{
		if (this->IsNullSet())
			return obj.IsNullSet();
		return lb== obj.lb&& ub== obj.ub;
	}
	
	bool operator!=(const Interval& obj)const
	{
		return !(*this==obj);
	}

	bool Contain(float r)const
	{
		return r >= lb&& r <= ub;
	}
};



/** Ray is consist of origin point & direction
*/
struct Ray{
	Point origin;
	Vector dir;
	Interval range;

	Ray(const Point& origin_ = Point(), const Vector& direction_ = Vector(),
		Interval range_ = Interval::NonNegtive())
		:origin(origin_), dir(direction_), range(range_)
	{
	}
	Point PointOnT(float t)const
	{
		return origin + t * dir;
	}
	float TOnPoint(const Point& point)const
	{
		Vector d = (point - origin) / dir;
		return (d.x + d.y + d.z) / 3.0f;
	}
};



/** Box is an object that can be hit and occupy an area of space
*/
class Box{
public:
	Box(const Point& a, const Point& b)
		:min_point_(
			a.x <= b.x ? a.x : b.x,
			a.y <= b.y ? a.y : b.y,
			a.z <= b.z ? a.z : b.z
		),
		max_point_(
			a.x >= b.x ? a.x : b.x,
			a.y >= b.y ? a.y : b.y,
			a.z >= b.z ? a.z : b.z
		)
	{
	}

	// check point inside
	bool Inside(const Point& p)const;

	// intersect test with a ray
	float Intersect(const Ray& ray) const;

	Box Union(const Box& box) const;

	Vector Size() { return max_point_ - min_point_; }

	float dx()const { return max_point_.x - min_point_.x; }
	float dy()const { return max_point_.y - min_point_.y; }
	float dz()const { return max_point_.z - min_point_.z; }

	float minx()const { return min_point_.x; }
	float miny()const { return min_point_.y; }
	float minz()const { return min_point_.z; }

	float maxx()const { return max_point_.x; }
	float maxy()const { return max_point_.y; }
	float maxz()const { return max_point_.z; }

	Point min_point()const { return min_point_; }
	Point max_point()const { return max_point_; }

	bool operator ==(const Box& other)const
	{
		return other.min_point_ == min_point_ && other.max_point_ == max_point_;
	}

private:
	Point min_point_, max_point_;

};


/** Transform automatically keep a transform matrix and its inverse matrix
*/
class Transform{
	Matrix m_, inv_m_;
public:
	Transform();
	Transform& Scale(float sx, float sy, float sz);

	Transform& RotateX(float phi);
	Transform& RotateY(float phi);
	Transform& RotateZ(float phi);
	Transform& Translation(float tx, float ty, float tz);
	Transform& Translation(const Vector& t);

	Point Apply(const Point& p)const;
	Vector Apply(const Vector& v)const;
	// NOTE: the transformation of normal is different
	// reference: Fundamentals of Computer Graphics Chapter 6.2.2
	Vector NormalApply(const Vector& normal)const;
	Ray Apply(const Ray& ray)const;
	Box Apply(const Box& box)const;

	Point Inverse(const Point& p)const;
	Vector Inverse(const Vector& v)const;
	Vector NormalInverse(const Vector& normal)const;
	Ray Inverse(const Ray& ray)const;
	Box Inverse(const Box& box)const;

};

inline std::ostream& operator<<(std::ostream& os, const Point& vol)
{
	os << '(' << vol.x << ',' << vol.y << ',' << vol.z << ')';
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Box& b)
{
	os << '[' << b.min_point() << " - " << b.max_point() << ']';
	return os;
}

inline std::ostream& operator<<(std::ostream& os, const Matrix& vol)
{
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			os << vol(i, j);
			if (j < 3)
				os << ' ';
		}
		if (i < 3)
			os << std::endl;
	}
	return os;
}

}//namespace ltray

#endif
