#include "geometry.h"

using namespace ltray;

// check point inside
bool Box::Inside(const Point& p)const
{

	return p.x >= minx()
		&& p.y >= miny()
		&& p.z >= minz()

		&& p.x <= maxx()
		&& p.y <= maxy()
		&& p.z <= maxz();
}

// intersect test with a ray
float Box::Intersect(const Ray& ray) const
{
	Point origin = ray.origin;
	Point minp = min_point();
	Point maxp = max_point();
	Vector dir = ray.dir;
	Interval range = ray.range;

	// get intersect interval for three axis
	for (int axis = 0; axis < 3; axis++){
		float a = (minp[axis] - origin[axis]) / dir[axis];
		float b = (maxp[axis] - origin[axis]) / dir[axis];
		if (a > b){
			float tmp = a;
			a = b;
			b = tmp;
		}

		range = range.Intersect(Interval(a, b));
		Point pa = ray.PointOnT(range.lb);
		Point pb = ray.PointOnT(range.ub);

		if (range.IsNullSet())
			return NAN;
	}
	return range.lb;
}

Box Box::Union(const Box& box) const
{
	Point pmin(
		minx() < box.minx() ? minx() : box.minx(),
		miny() < box.miny() ? miny() : box.miny(),
		minz() < box.minz() ? minz() : box.minz());
	Point pmax(
		maxx() > box.maxx() ? maxx() : box.maxx(),
		maxy() > box.maxy() ? maxy() : box.maxy(),
		maxz() > box.maxz() ? maxz() : box.maxz());

	return Box(pmin, pmax);
}


Transform::Transform()
:m_(Matrix::IdentityMatrix()), inv_m_(Matrix::IdentityMatrix())
{

}
Transform& Transform::Scale(float sx, float sy, float sz)
{
	m_ = m_ * Matrix::ScaleMatrix(sx, sy, sz);
	inv_m_ = Matrix::ScaleMatrix(1.0f / sx, 1.0f / sy, 1.0f / sz) * inv_m_;
	return *this;
}

Transform& Transform::RotateX(float phi)
{
	m_ = m_ * Matrix::RotXMatrix(phi);
	inv_m_ = Matrix::RotXMatrix(-phi) * inv_m_;
	return *this;
}
Transform& Transform::RotateY(float phi)
{
	m_ = m_ * Matrix::RotYMatrix(phi);
	inv_m_ = Matrix::RotYMatrix(-phi) * inv_m_;
	return *this;
}
Transform& Transform::RotateZ(float phi)
{
	m_ = m_ * Matrix::RotZMatrix(phi);
	inv_m_ = Matrix::RotZMatrix(-phi) * inv_m_;
	return *this;
}

Transform& Transform::Translation(float tx, float ty, float tz)
{
	m_ = m_ * Matrix::TranslateMatrix(tx, ty, tz);
	inv_m_ = Matrix::TranslateMatrix(-tx, -ty, -tz) * inv_m_;
	return *this;
}
Transform& Transform::Translation(const Vector& t)
{
	m_ = m_ * Matrix::TranslateMatrix(t.x, t.y, t.z);
	inv_m_ = Matrix::TranslateMatrix(-t.x, -t.y, -t.z) * inv_m_;
	return *this;
}

Point Transform::Apply(const Point& p)const
{
	return m_ * p;
}

Vector Transform::Apply(const Vector& v)const
{
	return m_ * v;
}

Vector Transform::NormalApply(const Vector& normal)const 
{
	return inv_m_.T() * normal;
}

Ray Transform::Apply(const Ray& ray)const
{
	return Ray(m_ * ray.origin, m_ * ray.dir, ray.range);
}


Box Transform::Apply(const Box& box)const
{
	// note: we can't just simply transform two vertex

	Box b1 = Box(m_ * Point(box.minx(), box.miny(), box.minz()),
		m_ * Point(box.maxx(), box.miny(), box.minz()));
	Box b2 = Box(m_ * Point(box.minx(), box.maxy(), box.minz()),
		m_ * Point(box.minx(), box.miny(), box.maxz()));
	Box b3 = Box(m_ * Point(box.minx(), box.maxy(), box.maxz()),
		m_ * Point(box.maxx(), box.maxy(), box.minz()));
	Box b4 = Box(m_ * Point(box.maxx(), box.miny(), box.maxz()),
		m_ * Point(box.maxx(), box.maxy(), box.maxz()));
	return (b1.Union(b2)).Union(b3.Union(b4));
}

Point Transform::Inverse(const Point& p)const
{
	return inv_m_ * p;
}

Vector Transform::Inverse(const Vector& v)const
{
	return inv_m_ * v;
}

Vector Transform::NormalInverse(const Vector& normal)const
{
	return m_.T() * normal;
}

Ray Transform::Inverse(const Ray& ray)const
{
	// NOTE: the Ray member t does not need to change
	// and keep the direction's length not be united
	return Ray(inv_m_ * ray.origin, inv_m_ * ray.dir, ray.range);
}

Box Transform::Inverse(const Box& box)const
{
	return (Box(inv_m_ * Point(box.minx(), box.miny(), box.minz()),
		inv_m_ * Point(box.maxx(), box.miny(), box.minz())).Union(
		Box(inv_m_ * Point(box.minx(), box.maxy(), box.minz()),
		inv_m_ * Point(box.minx(), box.miny(), box.maxz()))))\
		.Union(
		(Box(inv_m_ * Point(box.minx(), box.maxy(), box.maxz()),
		inv_m_ * Point(box.maxx(), box.maxy(), box.minz())).Union(
		Box(inv_m_ * Point(box.maxx(), box.miny(), box.maxz()),
		inv_m_ * Point(box.maxx(), box.maxy(), box.maxz())))));
}

