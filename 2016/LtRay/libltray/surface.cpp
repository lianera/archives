#include <algorithm>
#include <cmath>
#include <assert.h>
#include "surface.h"

using namespace ltray;

Sphere::Sphere()
:r_(1.0f), zmin_(-1.0f), zmax_(1.0f), phi_max_(2*PI)
{

}

Sphere::Sphere(float r)
: r_(r), zmin_(-r), zmax_(r), phi_max_(2*PI)
{
	assert(r > 0);
}

Sphere::Sphere(float r, float zmin, float zmax, float phi_max)
: r_(r), zmin_(zmin), zmax_(zmax), phi_max_(phi_max)
{
	assert(r > 0);
	assert(phi_max > 0);

	theta_min_ = acos(zmin);
	theta_max_ = acos(zmax);
}

Sphere::~Sphere()
{

}

Box Sphere::BoundingBox()const 
{
	return Box(Point(-r_, -r_, zmin_), Point(r_, r_, zmax_));
}

float Sphere::Hit(const Ray& ray)const
{
	Vector d = ray.dir;
	Vector e = ray.origin;	// as a Vector, convenient for calculation
	//(dx^2 + dy^2 + dz^2) t^2 + t (2 dx x0 + 2 dy y0 + 2 dz z0) -r ^ 2 + x0 ^ 2 + y0 ^ 2 + z0 ^ 2
	float A = d.Dot(d);
	float B = 2*d.Dot(e);
	float C = e.Dot(e) - r_*r_;
	
	float discriminant = B*B - 4*A*C;
	if (discriminant >= 0){
		float t1 = 0.5f * (-B - sqrt(discriminant)) / A;
		float t2 = 0.5f * (-B + sqrt(discriminant)) / A;
		// note t1 < t2 always true

		if (t1 >= ray.range.ub || t2 <= ray.range.lb)
			return NAN;
		
		if (t1 > ray.range.lb){
			Point hitp = ray.origin + t1 * ray.dir;
			// partial check
			if (hitp.z <= zmax_ && hitp.z >= zmin_){ // check z
				float phi = atan2(hitp.y, hitp.x); // check phi
				if (phi < 0)
					phi += 2 * PI;
				if (phi <= phi_max_){
					// hit t1
					return t1;
				}
			}
			
		}
		if (t2 < ray.range.ub){
			Point hitp = ray.origin + t2 * ray.dir;
			// partial check
			if (hitp.z <= zmax_ && hitp.z >= zmin_){ // check z
				float phi = atan2(hitp.y, hitp.x); // check phi
				if (phi < 0)
					phi += 2 * PI;
				if (phi <= phi_max_){
					// hit t2
					return t2;
				}
			}
		}
	}
	return NAN;
}

Vector Sphere::NormalOn(const Point& p)const
{
	return Vector(p).Normalized();
}




Plane::Plane()
:normal_(0.0f, 0.0f, 1.0f), d_(0.0f)
{
}

Plane::Plane(Vector normal, float d)
:normal_(normal), d_(d)
{
	assert(normal.IsNormalized());
}

Plane::~Plane()
{

}

Box Plane::BoundingBox()const
{
	float x = abs(normal_.x), y = abs(normal_.y), z = abs(normal_.z);
	if (y < EPSILON && z < EPSILON){
		return Box(Point(d_, -INF, -INF), Point(d_, INF, INF));
	}
	if (x < EPSILON && z < EPSILON){
		return Box(Point(-INF, d_, -INF), Point(INF, d_, INF));
	}
	if (x < EPSILON && y < EPSILON){
		return Box(Point(-INF, -INF, d_), Point(INF, INF, d_));
	}
	return Box(Point(-INF, -INF, -INF), Point(INF, INF, INF));
}

float Plane::Hit(const Ray& ray)const
{
	float t = (d_ - (ray.origin - Point(0.0f, 0.0f, 0.0f)).Dot(normal_)) / (ray.dir.Dot(normal_));
	if (t >= ray.range.lb && t <= ray.range.ub){
		return t;
	}
	return NAN;
}

Vector Plane::NormalOn(const Point& p)const
{
	return normal_;
}





Triangle::Triangle()
:a_(1.0f, 0.0f, 0.0f), b_(0.0f, 0.0f, 0.0f), c_(0.0f, 1.0f, 0.0f)
{
	ComputeNormal();
}

Triangle::Triangle(const Point& a, const Point& b, const Point& c)
: a_(a), b_(b), c_(c)
{
	ComputeNormal();
}

Triangle::~Triangle()
{

}

void Triangle::CalcMinMax(float a, float b, float c, float& min, float& max)const
{
	if (a > b){
		max = a;
		min = b;
	}
	else{
		max = b;
		min = a;
	}
	if (max < c)
		max = c;
	if (min > c)
		min = c;

}

Box Triangle::BoundingBox()const
{
	float minx_, maxx_, miny_, maxy_, minz_, maxz_;

	CalcMinMax(a_.x, b_.x, c_.x, minx_, maxx_);
	CalcMinMax(a_.y, b_.y, c_.y, miny_, maxy_);
	CalcMinMax(a_.z, b_.z, c_.z, minz_, maxz_);

	return Box(Point(minx_, miny_, minz_), Point(maxx_, maxy_, maxz_));
}


float Triangle::Hit(const Ray& ray)const
{
	float a = a_.x - b_.x;
	float b = a_.y - b_.y;
	float c = a_.z - b_.z;
	float d = a_.x - c_.x;
	float e = a_.y - c_.y;
	float f = a_.z - c_.z;
	float g = ray.dir.x;
	float h = ray.dir.y;
	float i = ray.dir.z;
	float j = a_.x - ray.origin.x;
	float k = a_.y - ray.origin.y;
	float l = a_.z - ray.origin.z;

	float M = a*(e*i - h*f) + b*(g*f - d*i) + c*(d*h - e*g);

	float t = -(f*(a*k - j*b) + e*(j*c - a*l) + d*(b*l - k*c)) / M;
	if (t < ray.range.lb || t > ray.range.ub)
		return NAN;

	float gamma = (i*(a*k - j*b) + h*(j*c - a*l) + g*(b*l - k*c)) / M;
	if (gamma < 0.0f || gamma > 1.0f)
		return NAN;

	float beta = (j*(e*i - h*f) + k*(g*f - d*i) + l*(d*h - e*g)) / M;
	if (beta < 0.0f || beta > 1 - gamma)
		return NAN;

	return t;
}

Vector Triangle::NormalOn(const Point& p)const
{
	return normal_;
}



Cone::Cone()
:r_(1.0f), h_(1.0f), zmax_(1.0f), phi_max_(2 * PI)
{
	
}

Cone::Cone(float r, float h)
: r_(r), h_(h), zmax_(h), phi_max_(2 * PI)
{
	assert(r > 0);
	assert(h > 0);
}

Cone::Cone(float r, float h, float zmax, float phi_max)
: r_(r), h_(h), zmax_(zmax), phi_max_(phi_max)
{
	assert(r > 0);
	assert(h > 0);
	assert(zmax > 0);
	assert(phi_max > 0);
}

Cone::~Cone()
{

}

Box Cone::BoundingBox()const
{
	return Box(Point(-r_, -r_, 0.0f), Point(r_, r_, zmax_));
}

float Cone::Hit(const Ray& ray)const
{
	Vector d = ray.dir;
	Vector e = ray.origin;	// as a Vector, convenient for calculation

	//ref math/draft1

	double sq_h_div_r = (double)h_*h_ / ((double)r_*r_);// h^2 / r^2;

	double A = ((double)d.x*d.x + d.y*d.y)*sq_h_div_r - (double)d.z*d.z;
	double B = 2.0 * (((double)d.x*e.x + d.y*e.y)*sq_h_div_r + d.z*((double)h_ - e.z));
	double C = ((double)e.x*e.x + e.y*e.y)*sq_h_div_r - (h_ - e.z)*((double)h_ - e.z);

	double discriminant = B*B - 4 * A*C;
	if (discriminant >= 0){
		
		float t1 = float(0.5 * (-B - sqrt(discriminant)) / A);
		float t2 = float(0.5 * (-B + sqrt(discriminant)) / A);
		// IMPORTANT t1 < t2 NOT always true
		if (t1 > t2){
			float tmp = t1;
			t1 = t2;
			t2 = tmp;
		}
		if (t1 >= ray.range.ub || t2 <= ray.range.lb)
			return NAN;

		if (t1 > ray.range.lb){
			Point hitp = ray.origin + t1 * ray.dir;
			// partial check
			if (hitp.z <= zmax_ && hitp.z >= 0){ // check z
				float phi = atan2(hitp.y, hitp.x); // check phi
				if (phi < 0)
					phi += 2 * PI;
				if (phi <= phi_max_){
					// hit t1
					return t1;
				}
			}

		}
		if (t2 < ray.range.ub){
			Point hitp = ray.origin + t2 * ray.dir;
			// partial check
			if (hitp.z <= zmax_ && hitp.z >= 0){ // check z
				float phi = atan2(hitp.y, hitp.x); // check phi
				if (phi < 0)
					phi += 2 * PI;
				if (phi <= phi_max_){
					// hit t2
					return t2;
				}
			}
		}
	}
	return NAN;
}

Vector Cone::NormalOn(const Point& p)const
{
	float nx = p.x;
	float ny = p.y;
	float nz = sqrt(nx*nx + ny*ny) * r_ / h_;
	return Vector(nx, ny, nz).Normalized();
}





Cylinder::Cylinder()
:r_(1.0f), zmin_(-1.0f), zmax_(1.0f), phi_max_(2 * PI)
{

}

Cylinder::Cylinder(float r, float height)
: r_(r), zmin_(-height / 2.0f), zmax_(height / 2.0f), phi_max_(2 * PI)
{
	assert(r > 0);
	assert(height > 0);
}

Cylinder::Cylinder(float r, float zmin, float zmax, float phi_max)
: r_(r), zmin_(zmin), zmax_(zmax), phi_max_(phi_max)
{
	assert(r > 0);
	assert(phi_max > 0);
}

Cylinder::~Cylinder()
{

}

Box Cylinder::BoundingBox()const
{
	return Box(Point(-r_, -r_, zmin_), Point(r_, r_, zmax_));
}

float Cylinder::Hit(const Ray& ray)const
{
	Vector d = ray.dir;
	Vector e = ray.origin;	// as a Vector, convenient for calculation
	//(dx^2 + dy^2) t^2 + t (2 dx x0 + 2 dy y0) -r ^ 2 + x0 ^ 2 + y0 ^ 2
	float A = d.x*d.x + d.y*d.y;
	float B = 2 * (d.x*e.x + d.y*e.y);
	float C = e.x*e.x + e.y*e.y - r_*r_;

	float discriminant = B*B - 4 * A*C;
	if (discriminant >= 0){
		float t1 = 0.5f * (-B - sqrt(discriminant)) / A;
		float t2 = 0.5f * (-B + sqrt(discriminant)) / A;
		// note t1 < t2 always true

		if (t1 >= ray.range.ub || t2 <= ray.range.lb)
			return NAN;

		if (t1 > ray.range.lb){
			Point hitp = ray.origin + t1 * ray.dir;
			// partial check
			if (hitp.z <= zmax_ && hitp.z >= zmin_){ // check z
				float phi = atan2(hitp.y, hitp.x); // check phi
				if (phi < 0)
					phi += 2 * PI;
				if (phi <= phi_max_){
					// hit t1
					return t1;
				}
			}

		}
		if (t2 < ray.range.ub){
			Point hitp = ray.origin + t2 * ray.dir;
			// partial check
			if (hitp.z <= zmax_ && hitp.z >= zmin_){ // check z
				float phi = atan2(hitp.y, hitp.x); // check phi
				if (phi < 0)
					phi += 2 * PI;
				if (phi <= phi_max_){
					// hit t2
					return t2;
				}
			}
		}
	}
	return NAN;
}

Vector Cylinder::NormalOn(const Point& p)const
{
	return Vector(p.x, p.y, 0.0f).Normalized();
}




Disk::Disk()
:r_(1.0f), inner_r_(0.0f), h_(0.0f), phi_max_(2 * PI)
{

}

Disk::Disk(float r)
: r_(r), inner_r_(0.0f), h_(0.0f), phi_max_(2 * PI)
{
	assert(r > 0);
}

Disk::Disk(float r, float inner_radius, float height, float phi_max)
: r_(r), inner_r_(inner_radius), h_(height), phi_max_(phi_max)
{
	assert(r > 0);
	assert(inner_radius >= 0);
	assert(height >= 0);
	assert(phi_max > 0);

}

Disk::~Disk()
{

}

Box Disk::BoundingBox()const
{
	return Box(Point(-r_, -r_, h_), Point(r_, r_, h_));
}

float Disk::Hit(const Ray& ray)const
{
	Vector d = ray.dir;
	Vector e = ray.origin;	// as a Vector, convenient for calculation
	
	// calc t
	float t = (h_ - e.z) / d.z;
	if (t >= ray.range.ub || t <= ray.range.lb)
		return NAN;
	// check hit point
	Point hitp = ray.origin + t * ray.dir;
	float sqdist = hitp.x * hitp.x + hitp.y * hitp.y;
	if (sqdist > r_*r_ || sqdist < inner_r_*inner_r_)	// check r, inner r
		return NAN;
	float phi = atan2(hitp.y, hitp.x); // check phi
	if (phi < 0)
		phi += 2 * PI;
	if (phi > phi_max_)
		return NAN;

	return t;
}

Vector Disk::NormalOn(const Point& p)const
{
	return Vector(0.0f, 0.0f, 1.0f);
}




Polygon::Polygon(const std::vector<Point>& points)
:_points(points)
{
	PreProjection();
}

Polygon::~Polygon()
{

}

void Polygon::PreProjection()
{
	float minx_ = INF, miny_ = INF, minz_ = INF;
	
	float maxx_ = -INF, maxy_ = -INF, maxz_ = -INF;

	for (int i = 0; i < (int)_points.size(); i++){
		Point& p = _points[i];
		if (p.x < minx_)
			minx_ = p.x;
		if (p.y < miny_)
			miny_ = p.y;
		if (p.z < minz_)
			minz_ = p.z;

		if (p.x > maxx_)
			maxx_ = p.x;
		if (p.y > maxy_)
			maxy_ = p.y;
		if (p.z > maxz_)
			maxz_ = p.z;
	}

	float dx = maxx_ - minx_;
	float dy = maxy_ - miny_;
	float dz = maxz_ - minz_;

	// project to 2d
	if (dx < dy && dx < dz){
		// yz plane
		index_u_ = 1;
		index_v_ = 2;
	}
	else if (dy < dz){
		// xz plane
		index_u_ = 0;
		index_v_ = 2;
	}
	else{
		// xy plane
		index_u_ = 0;
		index_v_ = 1;
	}

}

Box Polygon::BoundingBox()const
{
	float minx_ = INF, maxx_ = -INF,
		miny_ = INF, maxy_ = -INF,
		minz_ = INF, maxz_ = -INF;
	for (int i = 0; i < (int)_points.size(); i++){
		if (_points[i].x > maxx_)
			maxx_ = _points[i].x;
		if (_points[i].x < minx_)
			minx_ = _points[i].x;

		if (_points[i].y > maxy_)
			maxy_ = _points[i].y;
		if (_points[i].y < miny_)
			miny_ = _points[i].y;

		if (_points[i].z > maxz_)
			maxz_ = _points[i].z;
		if (_points[i].z < minz_)
			minz_ = _points[i].z;

	}
	return Box(Point(minx_, miny_, minz_), Point(maxx_, maxy_, maxz_));
}

float Polygon::Hit(const Ray& ray)const
{
	Vector n = (_points[1] - _points[0]).Cross(_points[2] - _points[1]).Normalized();
	float t = (_points[0] - ray.origin).Dot(n) / (ray.dir.Dot(n));

	if (t < ray.range.lb || t > ray.range.ub)
		return NAN;

	Point hitp = ray.origin + t * ray.dir;
	float hitu = index_u_ == 0 ? hitp.x : hitp.y;
	float hitv = index_v_ == 1 ? hitp.y : hitp.z;

	int pnum = (int)_points.size();

	int hit_edge_num = 0;
	for (int i = 0; i < pnum; i++){
		const Point& p1 = _points[i];
		const Point& p2 = _points[(i + 1) % pnum];

		float u1 = index_u_ == 0 ? p1.x : p1.y;
		float v1 = index_v_ == 1 ? p1.y : p1.z;
		float u2 = index_u_ == 0 ? p2.x : p2.y;
		float v2 = index_v_ == 1 ? p2.y : p2.z;

		// check the ray intersection with segment
		float k = (hitv - v1) / (v2 - v1);
		float s = (u2 - u1) * (hitv - v1) / (v2 - v1) + u1 - hitu;

		if (k > 0.0f && k < 1.0f && s > 0.0f){
			// hit
			hit_edge_num++;
		}
	}
	if (hit_edge_num % 2){
		// odd	means inside polygon
		return t;
	}

	return NAN;
}

Vector Polygon::NormalOn(const Point& p)const
{
	return (_points[1] - _points[0]).Cross(_points[2] - _points[1]).Normalized();
}



