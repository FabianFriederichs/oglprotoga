#include "ResourceLoader.h"


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
	std::vector<GameObject> gameobjects;

	std::vector<glm::vec3> poss;
	std::vector<glm::vec2> uvs;
	std::vector<glm::vec3> norms;
	std::vector<GLuint> indices;
	std::vector<std::string> faceslines;

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
				std::string facestring;
				std::getline(linestream, facestring);
				trimstr(facestring);
				faceslines.push_back(facestring);
			}
			else if (label == "g")
			{

			}
			else if (label == "o")
			{

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
		return gameobjects;
	}

	//now process face strings
	
	
}


