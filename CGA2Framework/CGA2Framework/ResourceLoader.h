#pragma once
#include "headers.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Texture.h"
#include "Material.h"
#include "GameObject.h"
#include "util.h"
#include "dds.h"

class ResourceLoader
{
public:
	static GLuint loadDDSTexture2D(const std::string& _filepath);
	static GLuint loadCubeMap(	const std::string& _filepath_posx,
								const std::string& _filepath_negx,
								const std::string& _filepath_posy,
								const std::string& _filepath_negy,
								const std::string& _filepath_posz,
								const std::string& _filepath_negz);

	//Reads an obj file and creates GameObjects with meshes. Material information is ignored.
	//For each "g" command in obj there will be created a new mesh,
	//for each "o" command a new GameObject which contains the subsequent meshes.
	//Meshes contain positions, uvs and normals as given in the file.
	static std::vector<GameObject> loadOBJ(const std::string& _filepath);
private:
	ResourceLoader();
	~ResourceLoader();

	static const std::regex facef_v;
	static const std::regex facef_vvt;
	static const std::regex facef_vvn;
	static const std::regex facef_vvtvn;

	static bool parseFaceStringToMesh(std::string& _face, Mesh& _mesh);

	struct oface
	{
		int oid;
		int gid;
		std::string face;
	};	
};

