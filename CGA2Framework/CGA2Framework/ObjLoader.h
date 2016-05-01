#pragma once
#include "headers.h"
#include "GameObject.h"
class OBJLoader
{
public:
	OBJLoader();
	~OBJLoader();

	static std::vector<GameObject> loadOBJ(const std::string& _filepath);

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

