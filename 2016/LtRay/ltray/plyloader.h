#ifndef _PLYLOADER_H_
#define _PLYLOADER_H_

#include <string>
#include <vector>
#include <memory>
#include "ltray.h"

class PLYLoader{
public:
	PLYLoader(const std::string& filename, std::shared_ptr<ltray::Material> material,
		std::shared_ptr<ltray::Transform> transform)
		:filename_(filename), material_(material), transform_(transform)
	{
		Load(filename);
	}


	std::vector<std::shared_ptr<ltray::Object>> objects()const
	{
		return objects_;
	}

private:
	void Load(const std::string& filename);

	std::string filename_;
	std::vector<std::shared_ptr<ltray::Object>> objects_;
	std::shared_ptr<ltray::Material> material_;
	std::shared_ptr<ltray::Transform> transform_;
};

#endif