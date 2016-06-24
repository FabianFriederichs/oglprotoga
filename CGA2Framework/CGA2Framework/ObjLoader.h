#pragma once
#include "headers.h"
#include "util.h"
#include "Model.h"
class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	static std::vector<Model> loadOBJ(const std::string& _filepath);

private:
	static const std::regex facef_v;
	static const std::regex facef_vvt;
	static const std::regex facef_vvn;
	static const std::regex facef_vvtvn;

	struct oface
	{
		int oid;
		int gid;
		std::string face;
	};
};

