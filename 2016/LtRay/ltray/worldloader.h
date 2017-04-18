#ifndef WORLD_LOADER_H
#define WORLD_LOADER_H

#include <string>
#include <map>
#include <memory>
#include "ltray.h"

namespace cpptoml{
	class table;
}

class WorldLoader{
public:
	WorldLoader(std::string filename);
	std::shared_ptr<ltray::Tracer> tracer()
	{
		return tracer_;
	}
	std::shared_ptr<ltray::Camera> camera()
	{
		return camera_;
	}
	std::shared_ptr<ltray::Scene> scene()
	{
		return scene_;
	}
	std::shared_ptr<ltray::Shader> shader()
	{
		return shader_;
	}
private:

	std::shared_ptr<cpptoml::table> world_;	

	std::shared_ptr<ltray::Shader> shader_;
	std::shared_ptr<ltray::Tracer> tracer_;
	std::shared_ptr<ltray::Camera> camera_;
	std::shared_ptr<ltray::Scene> scene_;
	ltray::Color ambient_;


	void LoadCamera();
	void LoadScene();
	void LoadTracer();

	void LoadLight(std::string lightname);
	void LoadObject(std::string objname);

	std::shared_ptr<ltray::Transform>  LoadTransform(std::shared_ptr<cpptoml::table> t);
	std::shared_ptr<ltray::Material> LoadMaterial(std::shared_ptr<cpptoml::table> t);
	std::shared_ptr<ltray::SurfaceBase> LoadSurface(std::shared_ptr<cpptoml::table> t);

	// objects storage
	std::vector<std::shared_ptr<ltray::Transform> > transforms_;
	std::map<std::string, std::shared_ptr<ltray::SurfaceBase> > surfaces_;
	std::map<std::string, std::shared_ptr<ltray::Material> > materials_;
	std::vector<std::shared_ptr<ltray::Light> > lights_;
	std::vector<std::shared_ptr<ltray::Object> > objects_;
};

#endif