#include <assert.h>
#include "world.h"

using namespace ltray;

float Object::Hit(const Ray& ray)const
{
	// NOTE: t does not need to change & don't normalize direction vector 
	// global to local
	Ray local_ray = transform->Inverse(ray);
	// test bounding
	float box_intersect = surface->BoundingBox().Intersect(local_ray);
	
	if (!std::isfinite(box_intersect))
		return NAN;
	float surface_hit = surface->Hit(local_ray);
	return surface_hit;
}

Box Object::BoundaryBox()const
{
	return transform->Apply(surface->BoundingBox());
}

Vector Object::NormalOn(const Point& p)const
{
	Point local_point = transform->Inverse(p);
	Vector local_normal = surface->NormalOn(local_point);
	assert(local_normal.IsNormalized());
	return transform->NormalApply(local_normal).Normalized();
}



Camera::Camera(const Point& position, const Point& lookat, Vector up,
	float fov_angle, bool orthographic, int pixel_nx, int pixel_ny,
	const Interval& range)
	:position_(position),
	orthographic_(orthographic),
	pixel_nx_(pixel_nx),
	pixel_ny_(pixel_ny),
	range_(range),
	fd_(1.0f)
{
	//reference: Fundamentals of Computer Graphics
	w_ = (position - lookat).Normalized();
	u_ = up.Cross(w_).Normalized();
	v_ = w_.Cross(u_).Normalized();

	float ratio = float(pixel_nx_) / pixel_ny_;
	float halfw_idth = fd_ * tan(0.5f * fov_angle * PI / 180);
	viewport_left_ = -halfw_idth;
	viewport_right_ = halfw_idth;
	viewport_top_ = halfw_idth / ratio;
	viewport_bottom_ = -halfw_idth / ratio;

}

Camera::~Camera()
{
}

Matrix Camera::SceneToCamera()const
{
	Matrix rot = Matrix(
		u_.x, v_.x, w_.x, 0.0f,
		u_.y, v_.y, w_.y, 0.0f,
		u_.z, v_.z, w_.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f).T();	// for orthography matrix, transpose equal to inverse

	Matrix trans = Matrix::TranslateMatrix(
		-position_.x,
		-position_.y,
		-position_.z);
	return rot*trans;
}

void Camera::GenerateRay(float x, float y, Ray& ray)const
{
	assert(int(x) < pixel_nx_);
	assert(int(y) < pixel_ny_);
	
	// from bottom to top, left to right
	float u = viewport_left_ + (viewport_right_ - viewport_left_)* x / float(pixel_nx_);
	float v = viewport_bottom_ + (viewport_top_ - viewport_bottom_)* y / float(pixel_ny_);

	Point origin;
	Vector direction;
	if (orthographic_){
		direction = -w_.Normalized();
		origin = position_ + u * u_ + v * v_;
	}
	else{
		direction = (-fd_ * w_ + u * u_ + v * v_).Normalized();
		origin = position_;
	}
	ray = Ray(origin, direction, range_);
}

