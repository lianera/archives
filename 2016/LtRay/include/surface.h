#pragma once
#ifndef SURFACE_H
#define SURFACE_H

#include <tuple>
#include "geometry.h"

namespace ltray{

class SurfaceBase{
public:
	virtual float Hit(const Ray& ray)const = 0;
	virtual Vector NormalOn(const Point& p)const = 0;
	virtual Box BoundingBox()const = 0;
	virtual ~SurfaceBase(){}
};


class Triangle :
	public SurfaceBase{
public:
	Triangle();
	Triangle(const Point& a, const Point& b, const Point& c);
	~Triangle();
	float Hit(const Ray& ray)const;
	Box BoundingBox()const;
	Vector NormalOn(const Point& p)const;
private:
	Point a_, b_, c_;
	Vector normal_;
	void CalcMinMax(float a, float b, float c, float& min, float& max)const;
	void ComputeNormal()
	{
		// pre cache normal
		normal_ = ((a_ - c_).Cross(b_ - a_)).Normalized();
	}
};


class Disk :
	public SurfaceBase{

public:
	Disk();
	Disk(float r);
	Disk(float r, float inner_radius, float height, float phi_max);
	~Disk();
	float Hit(const Ray& ray)const;
	Box BoundingBox()const override;
	Vector NormalOn(const Point& p)const;
private:
	float r_;	// radius
	float inner_r_;	// inner circle to be cut off
	float h_;	// height (distance to the xOy plane)
	float phi_max_;
};



class Sphere
	: public SurfaceBase{
public:
	Sphere();
	Sphere(float r);
	Sphere(float r, float zmin, float zmax, float phi_max);
	~Sphere();
	float Hit(const Ray& ray)const;
	Box BoundingBox()const;
	Vector NormalOn(const Point& p)const;
private:
	float r_;
	float phi_max_;
	float zmin_, zmax_;
	float theta_min_, theta_max_;

};


class Cone : public SurfaceBase{
public:
	Cone();
	Cone(float r, float h);
	Cone(float r, float h, float zmax, float phi_max);
	~Cone();

	float Hit(const Ray& ray)const;
	Box BoundingBox()const;
	Vector NormalOn(const Point& p)const;
public:
	float r_;	// radius
	float h_;	// height
	float zmax_;
	float phi_max_;
};


class Cylinder
	:public SurfaceBase{

public:
	Cylinder();
	Cylinder(float r, float height);
	Cylinder(float r, float zmin, float zmax, float phi_max);
	~Cylinder();
	float Hit(const Ray& ray)const;
	Box BoundingBox()const;
	Vector NormalOn(const Point& p)const;
private:
	float r_;
	float phi_max_;
	float zmin_, zmax_;

};


class Plane :
	public SurfaceBase{

public:
	Plane();
	Plane(Vector normal, float d);
	~Plane();
	float Hit(const Ray& ray)const;
	Box BoundingBox()const;
	Vector NormalOn(const Point& p)const;
private:
	Vector normal_;
	float d_;	// the distance from O to plane

};


class Polygon :
	public SurfaceBase{

public:
	//Polygon();
	Polygon(const std::vector<Point>& points);
	~Polygon();

	float Hit(const Ray& ray)const;
	Box BoundingBox()const;
	Vector NormalOn(const Point& p)const;
private:
	std::vector<Point> _points;
	int index_u_, index_v_;

	void PreProjection();

};

}//namespace ltray

#endif
