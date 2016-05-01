#include "OBJLoader.h"
//static defs
const std::regex OBJLoader::facef_v = std::regex("[0-9]+");
const std::regex OBJLoader::facef_vvt = std::regex("[0-9]+\/[0-9]+");
const std::regex OBJLoader::facef_vvn = std::regex("[0-9]+\/\/[0-9]+");
const std::regex OBJLoader::facef_vvtvn = std::regex("[0-9]+\/[0-9]+\/[0-9]+");


OBJLoader::OBJLoader()
{
}


OBJLoader::~OBJLoader()
{
}

std::vector<GameObject> OBJLoader::loadOBJ(const std::string& _filepath)
{
	try
	{
		std::vector<GameObject> gameobjects;
		std::vector<glm::vec3> poss;
		std::vector<glm::vec2> uvs;
		std::vector<glm::vec3> norms;
		std::vector<std::vector<std::vector<std::string>>> rawfaces;

		int objectid = -1;
		int groupid = -1;

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
					if (linestream >> pos.x >> pos.y >> pos.z)
						poss.push_back(pos);
					else
					{
						std::cerr << "Wrong vertex format.";
						return std::vector<GameObject>();
					}
				}
				else if (label == "vt")
				{
					glm::vec2 uv;
					if (linestream >> uv.x >> uv.y)
						uvs.push_back(uv);
					else
					{
						std::cerr << "Wrong texture coordinate format.";
						return std::vector<GameObject>();
					}
				}
				else if (label == "vn")
				{
					glm::vec3 norm;
					if (linestream >> norm.x >> norm.y >> norm.z)
						norms.push_back(norm);
					else
					{
						std::cerr << "Wrong normal format.";
						return std::vector<GameObject>();
					}
				}
				else if (label == "f")
				{
					std::string face;
					std::getline(linestream, face);
					trimstr(face);
					if (rawfaces.size() == 0 || objectid >= rawfaces.size() || objectid == -1)
					{
						if (objectid == -1) objectid++;
						rawfaces.push_back(std::vector<std::vector<std::string>>());
					}
					if (rawfaces[objectid].size() == 0 || groupid >= rawfaces[objectid].size() || groupid == -1)
					{
						if (groupid == -1) groupid++;
						rawfaces[objectid].push_back(std::vector<std::string>());
					}
					rawfaces[objectid][groupid].push_back(face);
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

		if (poss.size() > 0)
			foundposs = true;
		if (uvs.size() > 0)
			founduvs = true;
		if (norms.size() > 0)
			foundnorms = true;

		//now process face strings

		std::vector<Mesh> meshes;
		GameObject gameobj;
		Mesh mesh;
		GLuint index = 0;

		for (auto o = rawfaces.begin(); o != rawfaces.end(); o++)		//iterate objects
		{
			gameobj = GameObject();
			for (auto g = o->begin(); g != o->end(); g++)				//iterate meshes / groups
			{
				mesh = Mesh();	//new empty mesh
				for (auto f = g->begin(); f != g->end(); f++)			//iterate faces
				{
					std::istringstream facestream(*f);
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

						//Add unqiue vertices and corresponding indices to mesh //extremely bad performance. Consider a hashtable cache
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
				if (mesh.getVertices().size() > 0)
					gameobj.addMesh(mesh);
			}
			if (gameobj.getMeshes().size() > 0)
				gameobjects.push_back(gameobj);
		}
		return gameobjects;
	}
	catch (std::exception& ex)
	{
		std::cerr << "An error occured while processing obj file.\r\n" << ex.what() << std::endl;
		return std::vector<GameObject>();
	}
}
