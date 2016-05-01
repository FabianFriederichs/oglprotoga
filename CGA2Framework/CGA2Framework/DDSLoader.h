#pragma once
#include "Texture.h"
#include "Image2D.h"

class Texture;
class DDSLoader
{
public:
	DDSLoader();
	~DDSLoader();

	static bool extractDDSHeader(std::ifstream& _file, DDS_HEADER& _header, DDS_HEADER_DXT10& _dx10ExtHeader, bool& _foundDDSHeader, bool& _foundDX10ExtHeader);
	static bool loadDDSTex(const std::string& _filepath, Texture& _texture);	//for loading directly out of texture class
	static Texture* loadDDSTex(const std::string& _filepath);					//for creating textures externally from files
};

