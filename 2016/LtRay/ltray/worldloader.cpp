#include <iostream>
#include "worldloader.h"
#include "cpptoml.h"

using namespace cpptoml;
using namespace std;
using namespace ltray;


#define BOL(table, key, var) auto (var) = (table)->get_as<bool>(key)
#define DBL(table, key, var) auto (var) = (table)->get_as<double>(key)
#define INT(table, key, var) auto (var) = (table)->get_as<int>(key)
#define STR(table, key, var) auto (var) = (table)->get_as<string>(key)
#define ARR(table, key, var) auto (var) = (table)->get_array_of<double>(key)

ltray::Point asP(vector<double> arr){ return ltray::Point((float)(arr)[0], (float)(arr)[1], (float)(arr)[2]); }
ltray::Vector asV(vector<double> arr){ return ltray::Vector((float)(arr)[0], (float)(arr)[1], (float)(arr)[2]); }
ltray::Color asC(vector<double> arr){ return ltray::Color((float)(arr)[0], (float)(arr)[1], (float)(arr)[2]); }
ltray::Point asP(cpptoml::option<vector<double> > arr){ return asP(*arr); }
ltray::Vector asV(cpptoml::option<vector<double> > arr){ return asV(*arr); }
ltray::Color asC(cpptoml::option<vector<double> > arr){ return asC(*arr); }

#define RAD(angle) ((angle) * PI / 180)

WorldLoader::WorldLoader(std::string filename)
:scene_(make_shared<Scene>())
{
	try{
		world_ = parse_file(filename);
		LoadCamera();
		LoadScene();
		LoadTracer();
	}
	catch (const exception& e){
		cerr << e.what() << endl;
		exit(1);
	}
}

void WorldLoader::LoadTracer()
{
	auto tracer = world_->get_table("tracer");
	if (!tracer)
		throw runtime_error("cannot find tracer");

	INT(tracer, "recursion", recursion).value_or(5);
	INT(tracer, "antialias", antialias).value_or(2);
	STR(tracer, "shader", shader_type).value_or("phong");
	
	if (shader_type == "depth"){
		shader_ = make_shared<DepthShader>(camera_->direction());
	}
	else if (shader_type == "normal") {
		shader_ = make_shared<NormalMapShader>(camera_.get());
	}
	else if (shader_type == "spherical_harmonics") {
		auto shtable = tracer->get_array_of<cpptoml::array>("sh_coefs");
		vector<Color> sh_coefs;
		for (const auto& c : *shtable){
			auto color = c->as_array()->get_array_of<double>();
			sh_coefs.push_back(asC(color));
		}
		shader_ = make_shared<SphericalHarmonicsShader>(camera_.get(), sh_coefs);
	}
	else if (shader_type == "albedo") {
		shader_ = make_shared<AlbedoShader>();
	}
	else {
		shader_ = make_shared<PhongShader>(camera_->direction(), ambient_);
	}
	
	shared_ptr<TraverserBase> traverser(new GridTraverser(scene_->objects));
	tracer_ = make_shared<Tracer>(shader_, traverser, recursion, antialias);
}


void WorldLoader::LoadCamera()
{
	auto cam = world_->get_table("camera");
	if (!cam)
		throw runtime_error("cannot find camera");

	ARR(cam, "position", position);
	ARR(cam, "lookat", lookat);
	ARR(cam, "up", up);
	DBL(cam, "fov", fov);
	ARR(cam, "range", range);
	ARR(cam, "background", background).value_or({ 0.0, 0.0, 0.0});
	BOL(cam, "orthographic", orthographic).value_or(false);
	ARR(cam, "film_size", film_size);
	
	Interval r = Interval((float)range->at(0), (float)range->at(1));
	camera_ = make_shared<Camera>(asP(position), asP(lookat), asV(up), float(*fov),
		orthographic, (int)film_size->at(0), (int)film_size->at(1), r);
}

void WorldLoader::LoadScene()
{
	// load scene
	auto lights = world_->get_qualified_array_of<string>("scene.lights");
	auto objects = world_->get_qualified_array_of<string>("scene.objects");
	
	// load lights
	for (auto light : *lights)
		LoadLight(light);

	// load objects
	for (auto obj : *objects)
		LoadObject(obj);

	// copy to scene
	for (const auto& light : lights_)
		scene_->lights.push_back(light.get());
	for (const auto& obj : objects_)
		scene_->objects.push_back(obj.get());
}

void WorldLoader::LoadLight(std::string lightname)
{	
	auto lt = world_->get_table("light")->get_table(lightname);
	if (!lt)
		throw runtime_error("cannot find light: " + lightname);
	STR(lt, "type", type);
	ARR(lt, "color", color);
	DBL(lt, "maxangle", maxangle);
	DBL(lt, "falloffangle", falloffangle);
	DBL(lt, "falloff", falloff);

	auto t = LoadTransform(lt);

	shared_ptr<Light> light;
	if (*type == "ambient_light"){
		ambient_ = asC(color);
		return;
	}
	else if (*type == "direction_light")
		light = make_shared<DirectionLight>(asC(color));
	else if (*type == "point_light")
		light = make_shared<PointLight>(asC(color));
	else if (*type == "spot_light"){
		float maxrad = float(RAD(*maxangle));
		float falloffrad = float(RAD(*falloffangle));
		light = make_shared<SpotLight>(asC(color), maxrad, falloffrad, float(*falloff));
	}
	else
		throw runtime_error("invalid light type: " + *type);
	light->setTransform(t.get());
	transforms_.push_back(t);
	lights_.push_back(light);
}

void WorldLoader::LoadObject(std::string objname)
{
	auto obj = world_->get_table("object")->get_table(objname);
	if (!obj)
		throw runtime_error("cannot find object: " + objname);

	STR(obj, "surface", surface);
	STR(obj, "material", material);
	shared_ptr<SurfaceBase> s;
	shared_ptr<Material> m;

	// load surface
	auto sur_it = surfaces_.find(*surface);
	if (sur_it == surfaces_.end()){
		auto sur_table = world_->get_table("surface")->get_table(*surface);
		if (!sur_table)
			throw runtime_error("cannot find surface: " + *surface);
		s = LoadSurface(sur_table);
		surfaces_[*surface] = s;
	}
	else
		s = sur_it->second;

	// load material
	auto mat_it = materials_.find(*material);
	if (mat_it == materials_.end()){
		auto mat_table = world_->get_table("material")->get_table(*material);
		if (!mat_table)
			throw runtime_error("cannot find material: " + *material);
		m = LoadMaterial(mat_table);
		materials_[*material] = m;
	}
	else
		m = mat_it->second;

	auto t = LoadTransform(obj);
	transforms_.push_back(t);
	objects_.push_back(make_shared<Object>(s.get(), m.get(), t.get()));
}


shared_ptr<Transform> WorldLoader::LoadTransform(shared_ptr<cpptoml::table> t)
{	
	ARR(t, "position", pos);
	ARR(t, "rotate", rot);
	ARR(t, "scale", s);
	auto transform = make_shared<Transform>();
	if (pos)
		transform->Translation(asV(pos));
	if (s)
		transform->Scale(float(s->at(0)), float(s->at(1)), float(s->at(2)));
	if (rot){
		transform->RotateX(float(RAD(rot->at(0))));
		transform->RotateY(float(RAD(rot->at(1))));
		transform->RotateZ(float(RAD(rot->at(2))));
	}

	transforms_.push_back(transform);
	return transform;
}

std::shared_ptr<ltray::Material> WorldLoader::LoadMaterial(shared_ptr<cpptoml::table> t)
{
	ARR(t, "ambient", ambient).value_or({0.0,0.0,0.0});
	ARR(t, "diffuse", diffuse).value_or({ 0.0, 0.0, 0.0 });
	ARR(t, "specular", specular).value_or({ 0.0, 0.0, 0.0 });
	ARR(t, "mirror", mirror).value_or({ 0.0, 0.0, 0.0 });
	DBL(t, "phong_exp", phong_exp).value_or(1.0);
	BOL(t, "refractive", refractive).value_or(false);
	DBL(t, "ior", ior).value_or(1.0);
	ARR(t, "attenuation", attenuation).value_or({0.0,0.0,0.0});
	
	auto material = make_shared<Material>(asC(ambient), asC(diffuse), asC(specular), asC(mirror),
		float(phong_exp), refractive, float(ior), asC(attenuation));
	return material;
}

shared_ptr<SurfaceBase> WorldLoader::LoadSurface(std::shared_ptr<cpptoml::table> t)
{
	STR(t, "type", type);
	DBL(t, "radius", radius).value_or(1.0);
	DBL(t, "height", height);
	DBL(t, "zmin", zmin).value_or(-1);
	DBL(t, "zmax", zmax).value_or(1);
	DBL(t, "phimax", phimax).value_or(360);
	DBL(t, "inner_radius", inner_radius).value_or(0.0);
	float r = float(radius);
	float phi_max = float(RAD(phimax));

	shared_ptr<SurfaceBase> s;
	if (*type == "sphere")
		s = make_shared<Sphere>(r, float(zmin), float(zmax), phi_max);
	else if (*type == "cone"){
		float h = float(height.value_or(1.0));
		s = make_shared<Cone>(r, h, float(zmax), phi_max);
	}
	else if (*type == "cylinder")
		s = make_shared<Cylinder>(r, float(zmin), float(zmax), phi_max);
	else if (*type == "disk"){
		float h = float(height.value_or(0.0));
		s = make_shared<Disk>(r, float(inner_radius), h, phi_max);
	}
	else if (*type == "triangle"){
		ARR(t, "p1", p1);
		ARR(t, "p2", p2);
		ARR(t, "p3", p3);
		s = make_shared<Triangle>(asP(p1), asP(p2), asP(p3));
	}
	else if (*type == "polygon"){
		auto vertices_table = t->get_array_of<cpptoml::array>("vertices");
		vector<Point> vertices;
		for (const auto& v : *vertices_table){
			auto pv = v->as_array()->get_array_of<double>();
			vertices.push_back(asP(pv));
		}
		s = make_shared<Polygon>(vertices);
	}
	else{
		throw runtime_error("unknow surface type: " + *type);
	}
	return s;
}