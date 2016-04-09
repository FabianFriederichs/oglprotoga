#pragma once
#include "headers.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Texture.h"
#include "Material.h"
#include "GameObject.h"
class ResourceLoader
{
public:
	static GLuint loadTexture2D(const std::string& _filepath);
	static GLuint loadCubeMap(	const std::string& _filepath_posx,
								const std::string& _filepath_negx,
								const std::string& _filepath_posy,
								const std::string& _filepath_negy,
								const std::string& _filepath_posz,
								const std::string& _filepath_negz);
	static GameObject loadOBJ(const std::string& _filepath);

private:
	ResourceLoader();
	~ResourceLoader();
};

