#include <algorithm>
#include <memory>
#include <assert.h>
#include "tracing.h"


using namespace ltray;
using std::shared_ptr;



VoxelDDA::VoxelDDA(const VoxelGrid& voxle_grid)
:voxel_grid_(voxle_grid)
{
}

void VoxelDDA::Traval(const Ray& ray, VoxelProcesser& voxelfunc)const
{
	// compute start point
	float rayt;
	const Box& bound = voxel_grid_.bound_;
	if (bound.Inside(ray.origin)){
		rayt = ray.range.lb;
	}
	else{
		float hitr = bound.Intersect(ray);
		if (!std::isfinite(hitr)){
			return;
		}
		rayt = hitr;
	}
	Point p0 = ray.PointOnT(rayt);

	// 3D DDA
	Point next;
	Vector delta;
	int step[3];
	int cur_voxel[3];
	int out[3];
	for (int axis = 0; axis < 3; axis++){
		// start voxel
		cur_voxel[axis] = voxel_grid_.GeoToVoxel(axis, p0);
		// step parameter
		if (ray.dir[axis] >= 0){
			next[axis] = rayt +
				(voxel_grid_.VoxelToGeo(axis, cur_voxel[axis] + 1) - p0[axis]) / ray.dir[axis];
			delta[axis] = voxel_grid_.voxel_size_[axis] / ray.dir[axis];
			step[axis] = 1;
			out[axis] = voxel_grid_.voxel_num_[axis];
		}
		else{
			next[axis] = rayt +
				(voxel_grid_.VoxelToGeo(axis, cur_voxel[axis]) - p0[axis]) / ray.dir[axis];
			delta[axis] = -voxel_grid_.voxel_size_[axis] / ray.dir[axis];
			step[axis] = -1;
			out[axis] = -1;
		}
	}
	// traval
	for (;;){
		// call function object
		int cur_id = voxel_grid_.VoxelId(cur_voxel[0], cur_voxel[1], cur_voxel[2]);
		bool contnu = voxelfunc(cur_id);
		if (!contnu)
			break;
		// next axis, referenced pbrt, page 208 second edition, the trick is amazing
		int bits = ((next[0] < next[1]) << 2) +
			((next[0] < next[2]) << 1) +
			((next[1] < next[2]));
		const int cmp_axis[8] = { 2, 1, 2, 1, 2, 2, 0, 0 };
		int step_axis = cmp_axis[bits];
		// next voxel
		if (ray.range.ub < next[step_axis])
			break;
		cur_voxel[step_axis] += step[step_axis];
		if (cur_voxel[step_axis] == out[step_axis])
			break;
		next[step_axis] += delta[step_axis];
	}
}

GridTraverser::GridTraverser(const std::vector<const Object*>& Objects)
:TraverserBase(Objects)
{
	InitVoxelGrid();

	voxel_dda_.reset(new VoxelDDA(*voxel_grid_));

	InitObjectContainedVoxels();
}

void GridTraverser::InitVoxelGrid()
{
	assert(objects_.size() > 0);

	float size_statics = 0;
	// whole scene bounding
	Box scenebox((*objects_.begin())->BoundaryBox());
	for_each(objects_.begin(), objects_.end(), [&](const Object* obj){
		scenebox = scenebox.Union(obj->BoundaryBox());
		size_statics += obj->BoundaryBox().dx()
			+ obj->BoundaryBox().dy()
			+ obj->BoundaryBox().dz();
	});
	// get average suface size
	size_statics /= (objects_.size() * 3);
	// adjustment
	size_statics *= 0.5f;
	// numbers
	int voxel_num[3];
	voxel_num[0] = int(scenebox.dx() / size_statics);
	voxel_num[1] = int(scenebox.dy() / size_statics);
	voxel_num[2] = int(scenebox.dz() / size_statics);

	std::for_each(voxel_num, voxel_num + 3, [](int& n){
		if (n < 1) n = 1;
		if (n > 64) n = 64;
	});
	// voxel grid
	voxel_grid_.reset(new VoxelGrid(scenebox, voxel_num));

}

void GridTraverser::InitObjectContainedVoxels()
{
	voxels_ = std::vector<ObjectContainedVoxel>(voxel_grid_->VoxelNum());

	for_each(objects_.begin(), objects_.end(), [&](const Object* obj){
		int vmin[3], vmax[3];
		Box b = obj->BoundaryBox();

		for (int axis = 0; axis < 3; axis++){
			vmin[axis] = voxel_grid_->GeoToVoxel(axis, b.min_point());
			vmax[axis] = voxel_grid_->GeoToVoxel(axis, b.max_point());
		}
		// add surfaces to voxel

		for (int vz = vmin[2]; vz <= vmax[2]; vz++)
		for (int vy = vmin[1]; vy <= vmax[1]; vy++)
		for (int vx = vmin[0]; vx <= vmax[0]; vx++){
			int id = voxel_grid_->VoxelId(vx, vy, vz);
			voxels_[id].push_back(obj);
		}
	});
}

std::list<const Object*>
GridTraverser::Traverse(const Ray& ray)const
{
	VoxelList voxel_rec;
	// use recorder to recoerd all throughed voxels
	voxel_dda_->Traval(ray, voxel_rec);

	std::list<const Object*> objects;
	for (auto& vid : voxel_rec.voxels_id_){
		// corresponding ObjectContainedVoxel
		const ObjectContainedVoxel& mcv = voxels_[vid];
		for (const auto& m : mcv) {
			objects.push_back(m);
		}
	}
	return objects;
}