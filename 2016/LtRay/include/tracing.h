#pragma once
#ifndef TRACING_H
#define TRACING_H

#include <vector>
#include <list>
#include <assert.h>
#include <functional>
#include "geometry.h"
#include "world.h"
#include "shader.h"

namespace ltray{

/** provide potential surfaces for each ray
*/
class TraverserBase{
public:
	TraverserBase(const std::vector<const Object*>& objects)
		:objects_(objects){}
	virtual ~TraverserBase(){}
	virtual std::list<const Object*>
		 Traverse(const Ray& ray)const = 0;
protected:
	const std::vector<const Object*>& objects_;
};




/** process voxels
*/
class VoxelProcesser{
public:
	virtual ~VoxelProcesser(){}
	/** record
	* @param id voxel id
	* @return whether continue or not
	*/
	virtual bool operator()(int id) = 0;
};

class VoxelGrid{
	friend class VoxelDDA;
public:
	VoxelGrid(const Box& bound, int voxel_num[3])
		:bound_(bound)
	{
		for (int i = 0; i < 3; i++){
			voxel_num_[i] = voxel_num[i];
			voxel_size_[i] = (bound_.max_point() - bound_.min_point())[i] / voxel_num_[i];
		}
	}
	int GeoToVoxel(int axis, const Point& point)const
	{
		int v = int((point[axis] - bound_.min_point()[axis]) / voxel_size_[axis]);
		if (v < 0)
			v = 0;
		else if (v >= voxel_num_[axis])
			v = voxel_num_[axis] - 1;
		return v;
	}

	float VoxelToGeo(int axis, int vp)const
	{
		return bound_.min_point()[axis] + vp * voxel_size_[axis];
	}

	int VoxelId(int x, int y, int z)const
	{
		return z * voxel_num_[0] * voxel_num_[1]
			+ y * voxel_num_[0]
			+ x;
	}
	void IdToVoxel(int id, int& x, int& y, int& z)const
	{
		int num_xy = voxel_num_[0] * voxel_num_[1];
		z = id / num_xy;
		int nleft = id - z * num_xy;
		y = nleft / voxel_num_[0];
		x = nleft % voxel_num_[0];
	}
	int VoxelNum()const
	{
		return voxel_num_[0] * voxel_num_[1] * voxel_num_[2];
	}
private:
	Box bound_;
	int voxel_num_[3];
	float voxel_size_[3];
};

/** Voxel digit differencial analyzer
*/
class VoxelDDA{
public:
	
	VoxelDDA(const VoxelGrid& voxle_grid);
	//Voxel Functor should be  operator()(int vx, int vy, int vz), and return bool, true means continue;

	void Traval(const Ray& ray, VoxelProcesser& voxelfunc)const;
private:
	VoxelGrid voxel_grid_;
};


typedef std::list<const Object*> ObjectContainedVoxel;

/** record voxels
*/
class VoxelList : public VoxelProcesser{
	friend class GridTraverser;
public:
	bool operator()(int id)
	{
		voxels_id_.push_back(id);
		return true;
	}
	std::list<int> voxels_id_;
};

class GridTraverser :public TraverserBase{	//non-copyable
public:
	GridTraverser(const std::vector<const Object*>& Objects);

	std::list<const Object*>
		Traverse(const Ray& ray)const;

	~GridTraverser()
	{
	}
private:
	std::unique_ptr<VoxelDDA> voxel_dda_;
	std::unique_ptr<VoxelGrid> voxel_grid_;
	std::vector<ObjectContainedVoxel> voxels_;

	void InitVoxelGrid();
	void InitObjectContainedVoxels();

};

/*void TracingResultProc(int image_x, int image_y, 
		float red, float green, float blue,
		float point_x, float point_y, float point_z)
*/
typedef std::function<void(int x, int y,
	float r, float g, float b, 
	float px, float py, float pz)> TracingResultProc;

namespace Tracing {
	// compute reflect direction
	static Vector ReflectDirection(const Vector& direction, const Vector& normal)
	{
		return (direction - 2 * (direction.Dot(normal)) * normal).Normalized();
	}

	// compute refraction direction
	static bool RefractDirection(const Vector& direction, const Vector& normal,
		float refractive_index, Vector& refracted_direction)
	{
		assert(direction.IsNormalized());
		assert(normal.IsNormalized());

		float inv_refract = 1.0f / refractive_index;
		float neg_cos_theta = direction.Dot(normal);

		float sqcos_phi = 1.0f - (1.0f - neg_cos_theta * neg_cos_theta) *
			(inv_refract * inv_refract);
		if (sqcos_phi < 0)
			return false;

		Vector bsin_phi = (direction - normal * (direction.Dot(normal))) * inv_refract;
		Vector ncos_phi = normal * std::sqrt(sqcos_phi);
		refracted_direction = (bsin_phi - ncos_phi).Normalized();
		return true;
	}
}

class Tracer{
	
public:
	Tracer(const std::shared_ptr<Shader> shader,
		const std::shared_ptr<TraverserBase> traverser,
		int mirror_recursion_depth, int aa_grid_num_);

	void Trace(const Scene& scene, const Camera& camera, 
		TracingResultProc result_proc)const;
private:
	int mirror_recursion_depth_;
	const std::shared_ptr<Shader> shader_;
	const std::shared_ptr<TraverserBase> traverser_;

	const Object* HitObject(const Ray& ray, float* hit_result = nullptr)const;
	// tracer the ray to get the color
	Color RayTrace(const Ray& ray, const Scene& scene, int mirror_depth, Point* hit_p = NULL)const;
	// recursive color: reflection, refraction
	Color RecursiveColor(const Point& hit_point, float t, const Vector& incident_dir, Vector& normal,
		const Material& material, float range_max, int mirror_depth, const Scene& scene)const;
	// deal with shadow
	std::vector<const Light*> ValidIllumes(const Point& hit_point, const Scene& scene)const;

	Color Shade(float hit_record,
		const std::vector<std::shared_ptr<Light>>& illumes) const;

	// anti-aliasing grid number
	int aa_grid_num_;

	const float TRACING_EPSILON = 1e-3f;
};

}//namespace ltray

#endif