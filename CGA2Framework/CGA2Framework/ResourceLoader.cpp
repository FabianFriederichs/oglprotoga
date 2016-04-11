#include "ResourceLoader.h"

//static defs
const std::regex ResourceLoader::facef_v = std::regex("[0-9]+");
const std::regex ResourceLoader::facef_vvt = std::regex("[0-9]+\/[0-9]+");
const std::regex ResourceLoader::facef_vvn = std::regex("[0-9]+\/\/[0-9]+");
const std::regex ResourceLoader::facef_vvtvn = std::regex("[0-9]+\/[0-9]+\/[0-9]+");

ResourceLoader::ResourceLoader()
{
}


ResourceLoader::~ResourceLoader()
{
}

GLuint ResourceLoader::loadTexture2D(const std::string& _filepath)
{
	return 0;
}

GLuint ResourceLoader::loadCubeMap(const std::string& _filepath_posx,
	const std::string& _filepath_negx,
	const std::string& _filepath_posy,
	const std::string& _filepath_negy,
	const std::string& _filepath_posz,
	const std::string& _filepath_negz)
{
	return 0;		//will be implemented later
}

std::vector<GameObject> ResourceLoader::loadOBJ(const std::string& _filepath)
{
	try
	{
		std::vector<GameObject> gameobjects;

		std::vector<glm::vec3> poss;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> norms;
		std::vector<ResourceLoader::oface> faceslines;

		int objectid = 0;
		int groupid = 0;

		bool foundposs = false;
		bool founduvs = false;
		bool foundnorms = false;

		//read all mesh data from file
		std::ifstream objfs(_filepath.c_str());
		if (objfs.is_open())
		{
			while (!objfs.eof())
			{
				std::string line;
				std::getline(objfs, line);
				std::istringstream linestream(line);
				std::string label;
				std::string dat;
				linestream >> label;
				if (label == "v")
				{
					glm::vec3 pos;
					linestream >> pos.x >> pos.y >> pos.z;
					poss.push_back(pos);
				}
				else if (label == "vt")
				{
					glm::vec2 uv;
					linestream >> uv.x >> uv.y;
					uvs.push_back(uv);
				}
				else if (label == "vn")
				{
					glm::vec3 norm;
					linestream >> norm.x >> norm.y >> norm.z;
					norms.push_back(norm);
				}
				else if (label == "f")
				{
					ResourceLoader::oface face;
					std::getline(linestream, face.face);
					trimstr(face.face);
					face.oid = objectid;
					face.gid = groupid;
					faceslines.push_back(face);
				}
				else if (label == "g")
				{
					groupid++;
				}
				else if (label == "o")
				{
					groupid = 0;
					objectid++;
				}
				else
				{
					continue;
				}
			}
			objfs.close();
		}
		else
		{
			std::cerr << "OBJ file could not be opened.";
			return gameobjects; //return empty vector
		}

		//now process face strings
		//objectid = (objectid > 0 ? 1 : 0);
		//groupid = (groupid > 0 ? 1 : 0);

		std::vector<Mesh> meshes;
		GameObject gameobj;
		Mesh mesh;
		GLuint index = 0;

		if (poss.size() > 0)
		{
			foundposs = true;
		}

		if (uvs.size() > 0)
		{
			founduvs = true;
		}

		if (norms.size() > 0)
		{
			foundnorms = true;
		}
		//hi
		for (int o = 0; o < objectid; o++)
		{
			for (int g = 0; g < groupid; g++)
			{
			}
		}

		for (std::vector<ResourceLoader::oface>::iterator it = faceslines.begin(); it != faceslines.end(); it++)
		{
			if (it->oid != objectid)								//if no objects exist no gameobject is created. FIX THIS
			{
				objectid = it->oid;
				groupid = 0;
				//save collected meshes into new gameobject
				gameobj.addMeshes(meshes);
				gameobjects.push_back(gameobj);
				gameobj = GameObject();
				meshes.clear();	//clear temporary mesh vector
			}

			if (it->gid != groupid)									//same for meshes and groups
			{
				groupid = it->gid;
				//save mesh into current gameobject
				meshes.push_back(mesh); //save mesh
				mesh = Mesh();			//make new emtpy mesh
				index = 0;
			}

			std::istringstream facestream(it->face);
			std::string f1, f2, f3;
			if (facestream >> f1 >> f2 >> f3)
			{
				Vertex v1;
				Vertex v2;
				Vertex v3;

				std::vector<int> nums;

				//vertex1
				if ((foundposs) && std::regex_match(f1, facef_v))										//		v
				{
					nums = extractInts(f1, '//');
					v1.setPosition(poss[nums[0] - 1]);
				}
				else if ((foundposs && founduvs) && std::regex_match(f1, facef_vvt))					//		v/vt
				{
					nums = extractInts(f1, '//');
					v1.setPosition(poss[nums[0] - 1]);
					v1.setUV(uvs[nums[1] - 1]);
				}
				else if ((foundposs && foundnorms) && std::regex_match(f1, facef_vvn))					//		v//vn
				{
					nums = extractInts(f1, '//');
					v1.setPosition(poss[nums[0] - 1]);
					v1.setNormal(norms[nums[1] - 1]);
				}
				else if ((foundposs && founduvs && foundnorms) && std::regex_match(f1, facef_vvtvn))	//		v/vt/vn
				{
					nums = extractInts(f1, '//');
					v1.setPosition(poss[nums[0] - 1]);
					v1.setUV(uvs[nums[1] - 1]);
					v1.setNormal(norms[nums[2] - 1]);
				}
				else
				{
					std::cerr << "Wrong face format.";
					return std::vector<GameObject>();
				}

				//vertex2
				if ((foundposs) && std::regex_match(f1, facef_v))										//		v
				{
					nums = extractInts(f2, '//');
					v2.setPosition(poss[nums[0] - 1]);
				}
				else if ((foundposs && founduvs) && std::regex_match(f1, facef_vvt))					//		v/vt
				{
					nums = extractInts(f2, '//');
					v2.setPosition(poss[nums[0] - 1]);
					v2.setUV(uvs[nums[1] - 1]);
				}
				else if ((foundposs && foundnorms) && std::regex_match(f1, facef_vvn))					//		v//vn
				{
					nums = extractInts(f2, '//');
					v2.setPosition(poss[nums[0] - 1]);
					v2.setNormal(norms[nums[1] - 1]);
				}
				else if ((foundposs && founduvs && foundnorms) && std::regex_match(f1, facef_vvtvn))	//		v/vt/vn
				{
					nums = extractInts(f2, '//');
					v2.setPosition(poss[nums[0] - 1]);
					v2.setUV(uvs[nums[1] - 1]);
					v2.setNormal(norms[nums[2] - 1]);
				}
				else
				{
					std::cerr << "Wrong face format.";
					return std::vector<GameObject>();
				}

				//vertex3
				if ((foundposs) && std::regex_match(f1, facef_v))										//		v
				{
					nums = extractInts(f3, '//');
					v3.setPosition(poss[nums[0] - 1]);
				}
				else if ((foundposs && founduvs) && std::regex_match(f1, facef_vvt))					//		v/vt
				{
					nums = extractInts(f3, '//');
					v3.setPosition(poss[nums[0] - 1]);
					v3.setUV(uvs[nums[1] - 1]);
				}
				else if ((foundposs && foundnorms) && std::regex_match(f1, facef_vvn))					//		v//vn
				{
					nums = extractInts(f3, '//');
					v3.setPosition(poss[nums[0] - 1]);
					v3.setNormal(norms[nums[1] - 1]);
				}
				else if ((foundposs && founduvs && foundnorms) && std::regex_match(f1, facef_vvtvn))	//		v/vt/vn
				{
					nums = extractInts(f3, '//');
					v3.setPosition(poss[nums[0] - 1]);
					v3.setUV(uvs[nums[1] - 1]);
					v3.setNormal(norms[nums[2] - 1]);
				}
				else
				{
					std::cerr << "Wrong face format.";
					return std::vector<GameObject>();
				}

				//Add unqiue vertices and corresponding indices to mesh
				mesh.addIndicedVertex(v1);
				mesh.addIndicedVertex(v2);
				mesh.addIndicedVertex(v3);
			}
			else
			{
				//face not defined as triangle
				std::cerr << "Wrong face format.";
				return std::vector<GameObject>(); //return empty vector
			}
		}
		return gameobjects;
	}
	catch (std::exception& ex)
	{
		std::cerr << "An error occured while processing obj file.\r\n" << ex.what() << std::endl;
		return std::vector<GameObject>();
	}
}




