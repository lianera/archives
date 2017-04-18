#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <vector>
#include <algorithm>
#include <array>
#include <assert.h>
#include "world.h"


namespace ltray{

class Shader{
public:
	virtual bool shadow()const { return false; }
	virtual bool reflective()const { return false; };
	virtual bool refractive()const { return false; }
	virtual bool grayscale()const { return false; }

	virtual Color operator()(const Vector& normal, 
		const std::vector<const Light*>& illumes,
		const Object& object, const Ray& hit_ray, float hit_result)const = 0;
	virtual ~Shader(){}
};

class PhongShader
:public Shader{
public:
	PhongShader(const Vector& camera_direction, const Color& ambient_illume)
		:camera_direction_(camera_direction), ambient_illume_(ambient_illume)
	{}
	bool shadow()const { return true; }
	bool reflective()const { return true; };
	bool refractive()const { return true; }

	virtual Color operator()(const Vector& normal,
		const std::vector<const Light*>& illumes,
		const Object& object, const Ray& hit_ray, float hit_result)const
	{
		assert(normal.IsNormalized());
		const auto material = object.material;
		Color L = Color(0.0f, 0.0f, 0.0f);
		Point hit_point = hit_ray.PointOnT(hit_result);

		int illume_num = (int)illumes.size();
		for(const auto& illume : illumes){
			// skip the shadowed light
			if (!illume)
				continue;

			LightRay light_ray = illume->Radiance(hit_point);
			const Color& Ii = light_ray.color;
			Vector ldi = -light_ray.dir;
			assert(ldi.IsNormalized());
			// diffuse reflection
			L += material->diffuse*(Ii)* std::max(0.0f, normal.Dot(ldi));

			// specular reflection
			Vector h = (ldi + camera_direction_).Normalized();
			L += material->specular*(Ii)* powf(std::max(0.0f, normal.Dot(h)), material->phong_exp);
		}
		// ambient light
		L += material->ambient*(ambient_illume_);
		return L;
	}
private:
	const Vector camera_direction_;
	const Color ambient_illume_;
};

class AlbedoShader
	:public Shader {
public:

	virtual Color operator()(const Vector& normal,
		const std::vector<const Light*>& illumes,
		const Object& object, const Ray& hit_ray, float hit_result)const
	{
		return object.material->diffuse;
	}
};

class DepthShader
	:public Shader{
public:
	DepthShader(const Vector& camera_direction)
		:camera_direction_(camera_direction), Shader()
	{}
	bool grayscale()const { return true; }

	virtual Color operator()(const Vector& normal,
		const std::vector<const Light*>& illumes,
		const Object& object, const Ray& hit_ray, float hit_result)const
	{
		assert(camera_direction_.IsNormalized());
		float costheta = hit_ray.dir.Dot(camera_direction_);
		const float unit = 1000;// unit: mm
		return Color(hit_result * costheta) * 1000;
	}
private:
	Vector camera_direction_;
};

class NormalMapShader
	:public Shader{
	Camera* camera_;
public:
	NormalMapShader(Camera* camera) :camera_(camera){}

	virtual Color operator()(const Vector& normal,
		const std::vector<const Light*>& illumes,
		const Object& object, const Ray& hit_ray, float hit_result)const
	{
		assert(normal.IsNormalized());
		Vector norm = normal;
		norm = camera_->SceneToCamera()*norm;
		norm = norm * 0.5f + Vector(0.5f, 0.5f, 0.5f);
		return Color(norm.x, norm.y, norm.z);
	}
};

class SphericalHarmonicsShader
	:public Shader{
	Camera* camera_;
	std::vector<Color> coeffs_;
	bool simplify_;
public:		
	SphericalHarmonicsShader(Camera* camera, const std::vector<Color>& coeffs, bool simplify = false)
		:coeffs_(coeffs), camera_(camera), simplify_(simplify){}

	bool shadow()const { return true; }

	virtual Color operator()(const Vector& normal,
		const std::vector<const Light*>& illumes,
		const Object& object, const Ray& hit_ray, float hit_result)const
	{
		assert(normal.IsNormalized());
		Vector norm = camera_->SceneToCamera()*normal;
		float nx = norm.x;
		float ny = norm.y;
		float nz = norm.z;
		// Reference: Sloan P P. Stupid spherical harmonics (sh) tricks[C]//Game developers conference. 2008, 9.
		std::array<float, 9> y;
		if (simplify_)
			y = {1.0f, ny, nz, nx, nx*ny,
				ny*nz, -nx*nx - ny*ny + 2 * nz*nz,
				nz*nx, nx*nx - ny*ny};
		else
			y = {
			0.282095f,                              // 1/2 * Sqrt[1/Pi]
			-0.488603f * ny,                        // -Sqrt[3/(4*Pi)]
			0.488603f * nz,         // Sqrt[3/(4*Pi)]
			-0.488603f * nx,                        // -Sqrt[3/(4*Pi)]

			1.09255f * nx*ny,                       // 1/2*Sqrt[15/Pi]
			-1.09255f * ny*nz,                      // -1/2*Sqrt[15/Pi]
			0.315392f * (-nx*nx - ny*ny + 2 * nz*nz),         //1/4 * Sqrt[5/Pi]
			-1.09255f * nz*nx,                      // -1/2*Sqrt[15/Pi]
			1.09255f * (nx*nx - ny*ny)              // 1/2*Sqrt[15/Pi]
		};

		Color r;
		for(int i = 0; i < 9; i++)
			r += y[i] * coeffs_[i];
		const auto material = object.material;
		return r *material->diffuse;
	}		
};

}//namespace ltray

#endif