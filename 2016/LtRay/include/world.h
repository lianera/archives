#pragma once
#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <map>
#include <memory>

#include "utils.h"
#include "surface.h"

namespace ltray{


struct Color{
	float r, g, b;

	Color()
		:r(0.0f), g(0.0f), b(0.0f)
	{
	}
	Color(const std::vector<float>& vec)
		:r(vec[0]), g(vec[1]), b(vec[2])
	{
	}
	explicit Color(float intensity)
		:r(intensity), g(intensity), b(intensity)
	{
	}

	Color(float red, float green, float blue)
		:r(red), g(green), b(blue)
	{
	}

	bool operator!=(const Color& c)const
	{
		return r != c.r || g != c.g || b != c.b;
	}
	Color operator*(const Color& c)const
	{
		return Color(r*c.r, g*c.g, b*c.b);
	}

	Color& operator+=(const Color& c)
	{
		r += c.r;
		g += c.g;
		b += c.b;
		return *this;
	}

	static const Color& Black()
	{
		static const Color black(0.0f, 0.0f, 0.0f);
		return black;
	}
	static const Color& Gray()
	{
		static const Color gray(0.0f, 0.0f, 0.0f);
		return gray;
	}
	static const Color& White()
	{
		static const Color white(1.0f, 1.0f, 1.0f);
		return white;
	}
	static const Color& Red()
	{
		static const Color red(1.0f, 0.0f, 0.0f);
		return red;
	}
	static const Color& Green()
	{
		static const Color green(0.0f, 1.0f, 0.0f);
		return green;
	}
	static const Color& Blue()
	{
		static const Color blue(0.0f, 0.0f, 1.0f);
		return blue;
	}
	static const Color& Yellow()
	{
		static const Color yellow(1.0f, 1.0f, 0.0f);
		return yellow;
	}
	static const Color& Cyan()
	{
		static const Color cyan(0.0f, 1.0f, 1.0f);
		return cyan;
	}
	static const Color& Magenta()
	{
		static const Color magenta(1.0f, 0.0f, 1.0f);
		return magenta;
	}

	friend bool operator==(const Color& c1, const Color& c2);
	friend Color operator+(const Color& c1, const Color& c2);
	
	template <typename T>
	friend Color operator*(T a, const Color& c);
	template <typename T>
	friend Color operator*(const Color& c, T a);
};

inline bool operator==(const Color& c1, const Color& c2)
{
	return std::abs(c1.r - c2.r) < EPSILON &&
		std::abs(c1.g - c2.g) < EPSILON &&
		std::abs(c1.b - c2.b) < EPSILON;
}

template <typename T>
inline Color operator*(T a, const Color& c)
{
	return Color(a*c.r, a*c.g, a*c.b);
}

template <typename T>
inline Color operator*(const Color& c, T a)
{
	return Color(a*c.r, a*c.g, a*c.b);
}

inline Color operator+(const Color& c1, const Color& c2)
{
	return Color(c1.r + c2.r, c1.g + c2.g, c1.b + c2.b);
}



struct Material{
	Color ambient, diffuse,	specular, mirror; 
	float phong_exp;

	bool refractive;
	Color attenuation;	// the fall off parameter of dielectric
	float ior;

	Material(const Color& ambient_,
		const Color& diffuse_,
		const Color& specular_,
		const Color& mirror_,
		float phong_exp_,
		bool refractive_,
		float ior_,
		const Color& attenuation_ = Color::Black())
		:ambient(ambient_), diffuse(diffuse_),
		specular(specular_), mirror(mirror_), phong_exp(phong_exp_),
		refractive(refractive_), ior(ior_), attenuation(attenuation_)
	{}

	//--- getter ---
	bool reflective()const { return mirror != Color::Black(); }

};


/** A Object is a space entity, which has property of material and transforms
*/
struct Object{
	const Material* material;
	const SurfaceBase* surface;
	const Transform* transform;
	
	Object(const SurfaceBase* surface_,
		const Material* material_,
		const Transform* transform_)
		:surface(surface_), material(material_), transform(transform_)
	{
	}
	float Hit(const Ray& ray)const;
	Box BoundaryBox()const;
	Vector NormalOn(const Point& p)const;
};



struct LightRay {
	Color color;
	Vector dir;

	LightRay(const Color& color_, const Vector& direction_)
		:color(color_), dir(direction_) {}
	
	friend bool operator==(const LightRay& lr1, const LightRay& lr2);
};

inline bool operator==(const LightRay& lr1, const LightRay& lr2)
{
	return lr1.color == lr2.color && lr1.dir == lr2.dir;
}

/** what the difference between Light and Light is \
Light is global, while Light is local
*/
class Light {
public:
	Light()
	{
		const static Transform t;
		transform_ = &t;
	}
	virtual ~Light() {}

	void setTransform(const Transform* transform)
	{
		transform_ = transform;
	}

	LightRay Radiance(const Point& point)const
	{
		Point local_point = transform_->Inverse(point);
		LightRay lray = this->LocalRadiance(local_point);
		return LightRay(lray.color,
			transform_->Apply(lray.dir).Normalized());
	}
	float Distance(const Point& point)const
	{
		Point local_point = transform_->Inverse(point);
		float dist = this->LocalDistance(local_point);
		return dist;
	}
protected:
	// Note : On the local coordination, all light placed on(0, 0, 0), with direction towards(0, 0, 1)
	virtual LightRay LocalRadiance(const Point& p)const = 0;
	virtual float LocalDistance(const Point& p)const = 0;

private:
	const Transform* transform_;
};

class DirectionLight
	:public Light{
	Color color_;
public:
	DirectionLight(const Color& color)
		:color_(color){}

	virtual LightRay LocalRadiance(const Point& p)const override
	{
		return LightRay(color_, Vector(0.0f, 0.0f, 1.0f));
	}
	virtual float LocalDistance(const Point& p)const override
	{
		return INF;
	}
};

class PointLight
	:public Light{
	Color intensity_;
public:
	PointLight(const Color intensity)
		:intensity_(intensity){}
	virtual LightRay LocalRadiance(const Point& p)const
	{
		Vector vec = Vector(p);
		Color color = intensity_ * (1.0f / vec.SqSum());
		return LightRay(color, vec);
	}
	virtual float LocalDistance(const Point& p)const override
	{
		return Vector(p).Norm2();
	}
};

class SpotLight
	:public Light{
	Color intensity_;
	float max_rad_, falloff_rad_, falloff_param_;
public:
	SpotLight(const Color intensity, 
		float max_rad, float falloff_rad, float falloff_param)
		:intensity_(intensity),
		max_rad_(max_rad), falloff_rad_(falloff_rad), 
		falloff_param_(falloff_param)
		{}
	LightRay LocalRadiance(const Point& p)const
	{
		Vector vp = Vector(p);
		float costheta = p.z / vp.Norm2();

		// cosine is decreasing function
		if (costheta < std::cos(max_rad_))	// out of range
			return LightRay(Color(0.0f, 0.0f, 0.0f), vp);

		Color I = intensity_ * (1.0f / Vector(p).SqSum());

		if (costheta < std::cos(falloff_rad_)) {	// falloff area
													// compute falloff
			float falloff = std::pow((costheta - std::cos(max_rad_)) /
				(std::cos(falloff_rad_) - std::cos(max_rad_)), falloff_param_);
			return LightRay(I * falloff, vp);
		}
		// center area
		return LightRay(I, vp);
	}

	virtual float LocalDistance(const Point& p)const override
	{
		return Vector(p).Norm2();
	}
};



struct Scene{
	std::vector<const Light*> lights;
	std::vector<const Object*> objects;

	Color ambient_light;
	Color background;
};





/**
* camera is the ray source
*/
class Camera{
public:
	// Camera initialize, using position, lookat, up to describe the camera transformation
	Camera(const Point& position, const Point& lookat, Vector up,
		float fov_angle, bool orthographic,
		int pixel_nx_, int pixel_ny_, const Interval& range);
	~Camera();

	// the origin of camera
	Point position()const	
	{
		return position_;
	}

	// the direction of camera, note that the w_ axis is backwards direction
	Vector direction()const
	{
		return -w_;
	}
	// sight range of camera
	Interval range()const{ return range_; }

	// scene to camera
	Matrix SceneToCamera()const;

	// generate a ray
	void GenerateRay(float x, float y, Ray& ray)const;

	// film width
	int pixel_nx()const { return pixel_nx_; }

	// film height
	int pixel_ny()const { return pixel_ny_; }
private:
	Vector u_, v_, w_;	// coordination
	Point position_;

	float viewport_left_, viewport_right_, viewport_top_, viewport_bottom_;	// viewport size
	bool orthographic_;
	float fd_;		// focal distance
	int pixel_nx_, pixel_ny_;
	Interval range_;

};


}//namespace ltray

#endif