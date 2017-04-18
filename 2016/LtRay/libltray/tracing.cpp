#include <algorithm>
#include <iostream>
#include <iomanip> 
#include <ctime>
#include "tracing.h"

using namespace ltray;
using namespace std;

Tracer::Tracer(const shared_ptr<Shader> shader,
	const shared_ptr<TraverserBase> traverser,
	int mirror_recursion_depth, int aa_grid_num)
:shader_(shader), traverser_(traverser), aa_grid_num_(aa_grid_num),
mirror_recursion_depth_(mirror_recursion_depth)
{
}

const Object* Tracer::HitObject(const Ray& ray, float* hit_result)const
{
	// hit test
	auto objects = traverser_->Traverse(ray);
	const Object* hitobj = nullptr;
	float hit = INF;
	// select best hit
	for (auto& obj : objects){
		float obj_hit = obj->Hit(ray);
		if (obj_hit < hit){
			hit = obj_hit;
			hitobj = obj;
		}
	}
	if(hit_result)
		*hit_result = hit;
	return hitobj;
}


Color Tracer::RayTrace(const Ray& ray, const Scene& scene, int mirror_depth, Point* hit_p)const
{
	Ray cray = ray;
	if (mirror_depth == 0)	// the end of recursive
		return Color::Gray();
	// mirror or shadow
	if (mirror_depth != mirror_recursion_depth_)
		cray.range = Interval(TRACING_EPSILON, ray.range.ub);

	float t;
	const Object* obj = HitObject(cray, &t);
	Color color;
	// shading
	if (obj) {

		Point hit_point = cray.PointOnT(t);
		// get valid light because of shadow
		auto valid_illumes = ValidIllumes(hit_point, scene);

		// color shading
		Vector normal = obj->NormalOn(hit_point);
		color = (*shader_)(normal, valid_illumes, *obj, cray, t);


		// check mirror & refraction
		if (shader_->refractive() || shader_->reflective()) {
			Material material = *obj->material;
			if(material.reflective() || material.refractive){
				Vector incident_dir = cray.dir;
				Vector normal = obj->NormalOn(hit_point);
				color += RecursiveColor(hit_point, t, incident_dir, normal, material, ray.range.ub, mirror_depth - 1, scene);
			}
		}

		// set the out param of hit point
		if(hit_p)
			*hit_p = hit_point;
	}
	else{
		// clear color (background)
		color = scene.background;
		if(hit_p)
			*hit_p = Point(INF, INF, INF);
	}
	return color;
}

vector<const Light*> Tracer::ValidIllumes(const Point& hit_point, const Scene& scene)const
{
	vector<const Light*> valid_illumes;
	// check for shadow
	if (shader_->shadow()) {
		for (const auto& illume : scene.lights) {
			// detect obstacle between hit point and light source
			float dist = illume->Distance(hit_point);
			Ray light_ray(hit_point, -illume->Radiance(hit_point).dir,
				Interval(TRACING_EPSILON, dist));

			const Object* light_hit = HitObject(light_ray);
			if (!light_hit)
				valid_illumes.push_back(illume);
		};
	}
	else {
		valid_illumes = scene.lights;
	}
	return valid_illumes;
}

Color Tracer::RecursiveColor(const Point& hit_point, float t, const Vector& incident_dir, Vector& normal, 
	const Material& material, float range_max,  int mirror_depth, const Scene& scene)const
{
	assert(incident_dir.IsNormalized());
	assert(normal.IsNormalized());
	
	Color color;

	Vector reflect_dir = Tracing::ReflectDirection(incident_dir, normal);
	assert(reflect_dir.IsNormalized());
	Ray reflect_ray(hit_point, reflect_dir, Interval(TRACING_EPSILON, range_max));

	if (material.refractive && shader_->refractive()) {
		const float refract_index = material.ior;
		Vector refract_dir;

		// refraction
		float cos_theta = 0.0f;
		Color attenuation = material.attenuation;
		Color intensity;

		if (incident_dir.Dot(normal) < 0) {// air to dielectric
			Tracing::RefractDirection(incident_dir, normal, refract_index, refract_dir);
			cos_theta = -incident_dir.Dot(normal);
			intensity = Color(1.f, 1.f, 1.f);	// there is no attenuation in the air
		}
		else {// dielectric to air
			intensity = Color(std::exp(-attenuation.r * t),
				std::exp(-attenuation.g * t),
				std::exp(-attenuation.b * t));
			if (Tracing::RefractDirection(incident_dir, -normal,
				1.f / refract_index, refract_dir)) {	// ordinary refraction
				cos_theta = refract_dir.Dot(normal);
			}

			else {// total internal reflection
				return intensity * RayTrace(reflect_ray, scene, mirror_depth);
			}
		}
		// compute reflectance
		float reflectance_init = (refract_index - 1.0f) / (refract_index + 1.0f);
		reflectance_init *= reflectance_init;
		float reflectance = reflectance_init + (1.0f - reflectance_init)*pow(1.0f - cos_theta, 5);
		Color reflect_part;
		if(material.reflective() && shader_->reflective())
			reflect_part = reflectance * RayTrace(reflect_ray, scene, mirror_depth);

		Ray refract_ray(hit_point, refract_dir, Interval(TRACING_EPSILON, range_max));
		Color refract_part = (1.0f - reflectance) * RayTrace(refract_ray, scene, mirror_depth);

		return intensity * (reflect_part + refract_part);
	}
	else if (material.reflective() && shader_->reflective()) {	// only reflective
		Color k_m = material.mirror;
		color = k_m*(RayTrace(reflect_ray, scene, mirror_depth));
	}
	return color;
}

void Tracer::Trace(const Scene& scene, const Camera& camera,
	TracingResultProc result_proc)const
{
	// record tiem
	clock_t last_time = clock();

	// for each camera ray
	Ray cam_ray;
	for (int y = 0; y < camera.pixel_ny(); y++) {
		for (int x = 0; x < camera.pixel_nx(); x++) {	// O(nx*ny)
			Color color = Color::Black();
			Point hit_p = Point(0.0f, 0.0f, 0.0f);
			float filmx, filmy;
			if(aa_grid_num_ > 0){	// anti-aliasing
				int n = aa_grid_num_;
				// stratified sampling (for anti-aliasing) [expensive]
				for (int p = 0; p < n; p++){		
					for (int q = 0; q < n; q++){	// O(n^2)
						filmx = x + (float(p) + float(std::rand()) / float(RAND_MAX)) / float(n);
						filmy = y + (float(q) + float(std::rand()) / float(RAND_MAX)) / float(n);
						camera.GenerateRay(filmx, filmy, cam_ray);
						Point p;
						color += RayTrace(cam_ray, scene, mirror_recursion_depth_, &p);
						hit_p += p;
					}
				}
				float inv_num = 1.0f / (n * n);
				color = color * inv_num;
				hit_p = Point(hit_p.x * inv_num, hit_p.y * inv_num, hit_p.z * inv_num);				
			}
			else {	// O(1)
				// sampling center
				filmx = x + 0.5f;
				filmy = y + 0.5f;
				camera.GenerateRay(filmx, filmy, cam_ray);
				color = RayTrace(cam_ray, scene, mirror_recursion_depth_, &hit_p);
			}
			// transform points to camera coordination
			hit_p = camera.SceneToCamera() * hit_p;
			result_proc(x, y, color.r, color.g, color.b, 
								hit_p.x, hit_p.y, hit_p.z);
			// calculate time
			clock_t current_time = clock();
			int secs = int(double(current_time - last_time) / CLOCKS_PER_SEC);
			if (secs >= 3) {
				last_time = current_time;
				float percent = float(y * camera.pixel_nx() + x)/ float(camera.pixel_nx() * camera.pixel_ny());
				cout << fixed << setprecision(2) << percent * 100 << "%\t";
			}
		}
	}
	cout << endl;
}