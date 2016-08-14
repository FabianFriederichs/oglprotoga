#pragma once
#include "Texture2D.h"
#include "Image2D.h"
#include "TextureCB.h"
#include "dds.h"

class Texture;
class DDSLoader
{

private:
	static std::vector<Image2D> loadImageMipmaps(const std::string& _filepath);
public:
	DDSLoader();
	~DDSLoader();

	static bool extractDDSHeader(std::ifstream& _file, DDS_HEADER& _header, DDS_HEADER_DXT10& _dx10ExtHeader, bool& _foundDDSHeader, bool& _foundDX10ExtHeader);
	static bool loadDDSTex(const std::string& _filepath, Texture& _texture);	//for loading directly out of texture class
	static Texture* loadDDSTex(const std::string& _filepath);					//for creating textures externally from files
	static Texture* loadDDSTex(	const std::string& _filepathposx, const std::string& _filepathnegx,	//cubemap time!
								const std::string& _filepathposy, const std::string& _filepathmegy,
								const std::string& _filepathposz, const std::string& _filepathnegz);
};

