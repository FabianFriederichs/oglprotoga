#include "dds.h"

bool extractDDSHeader(std::ifstream& _file, DDS_HEADER& _header, DDS_HEADER_DXT10& _dx10ExtHeader, bool& _foundDDSHeader, bool& _foundDX10ExtHeader)
{	
	if (!_file.is_open())
		return false;

	char magicnumber[4];
	_file.read(magicnumber, 4);

	if (strncmp(magicnumber, "DDS ", 4) != 0)
	{
		return false;
	}

	bool success = true;

	//read in header

	if (!_file.read(reinterpret_cast<char*>(&(_header.dwSize)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwFlags)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwHeight)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwWidth)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwPitchOrLinearSize)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwDepth)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwMipMapCount)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[0])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[1])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[2])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[3])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[4])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[5])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[6])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[7])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[8])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[9])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved1[10])), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.ddspf.dwSize)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.ddspf.dwFlags)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.ddspf.dwFourCC)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.ddspf.dwRGBBitCount)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.ddspf.dwRBitMask)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.ddspf.dwGBitMask)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.ddspf.dwBBitMask)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.ddspf.dwABitMask)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwCaps)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwCaps2)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwCaps3)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwCaps4)), sizeof(DWORD))) success = false;
	if (!_file.read(reinterpret_cast<char*>(&(_header.dwReserved2)), sizeof(DWORD))) success = false;

	if (!success)
	{
		_foundDDSHeader = false;
		return false;
	}
	else
	{
		_foundDDSHeader = true;
	}

	//check for dx10 extension header
	if ((_header.ddspf.dwFlags == DDPF_FOURCC) && (reinterpret_cast<char*>(_header.ddspf.dwFourCC) == "DX10"))
	{
		if (!_file.read(reinterpret_cast<char*>(&(_dx10ExtHeader.dxgiFormat)), sizeof(DXGI_FORMAT))) success = false;
		if (!_file.read(reinterpret_cast<char*>(&(_dx10ExtHeader.resourceDimension)), sizeof(D3D10_RESOURCE_DIMENSION))) success = false;
		if (!_file.read(reinterpret_cast<char*>(&(_dx10ExtHeader.miscFlag)), sizeof(UINT))) success = false;
		if (!_file.read(reinterpret_cast<char*>(&(_dx10ExtHeader.arraySize)), sizeof(UINT))) success = false;
		if (!_file.read(reinterpret_cast<char*>(&(_dx10ExtHeader.miscFlags2)), sizeof(UINT))) success = false;

		if (!success)
		{
			_foundDX10ExtHeader = false;
			return false;
		}
		else
			_foundDX10ExtHeader = true;
	}

	return success;
}

//support only DXT1, DXT3 and DXT5 compression formats
Texture loadDDSTex(const std::string& _filepath)
{
	Texture tex;
	std::ifstream file(_filepath.c_str(), std::ifstream::binary);
	DDS_HEADER _header;
	DDS_HEADER_DXT10 _dx10ExtHeader;
	bool ddsHeaderFound = false;
	bool dx10ExtHeaderFound = false;
	TEXTYPE type;
	if (!extractDDSHeader(file, _header, _dx10ExtHeader, ddsHeaderFound, dx10ExtHeaderFound))
	{
		std::cerr << "DDS file is corrupted." << std::endl;
		file.close();
		return tex;
	}

	if (ddsHeaderFound)
	{
		// default
		type = TEX_2D;

		// cubemap?
		if ((_header.dwCaps & (DDSCAPS_COMPLEX)) && (_header.dwCaps2 & DDSCAPS2_CUBEMAP))
			type = TEX_CUBEMAP;

		// volume?
		if ((_header.dwCaps & (DDSCAPS_COMPLEX)) && (_header.dwCaps2 & DDSCAPS2_VOLUME) && (_header.dwFlags & DDSD_DEPTH) && (_header.dwDepth > 0))
			type = TEX_3D;
		
		switch (type)
		{
		case TEX_2D:
			if ((_header.dwFlags & DDSD_LINEARSIZE) && (_header.ddspf.dwFlags & DDPF_FOURCC)) //compressed texture
			{
				//format
				unsigned int components = 0;
				DWORD format;
				switch (_header.ddspf.dwFourCC)
				{
					case FOURCC_DXT1:
						format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
						components = 3;
						break;
					case FOURCC_DXT3:
						format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
						components = 4;
						break;
					case FOURCC_DXT5:
						format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
						components = 4;
						break;
					default:
						file.close();
						std::cerr << "DDS loader: Only DXT1, DXT3 and DXT5 compressed textures are supported." << std::endl;
						return tex;
						break;
				}

				if ((_header.dwFlags & DDSD_MIPMAPCOUNT) && (_header.dwCaps & DDSCAPS_COMPLEX)) //DDS contains mipmaps
				{
					unsigned long bufsize = 0;
					unsigned long curw = _header.dwWidth;
					unsigned long curh = _header.dwHeight;
					std::vector<std::vector<unsigned char>> data;
					for (int i = 0; i < _header.dwMipMapCount; i++)
					{
						bufsize = std::max(1UL, ((curw + 3) / 4)) * std::max(1UL, ((curh + 3) / 4)) * (_header.ddspf.dwFourCC == FOURCC_DXT1 ? 8 : 16);
						std::vector<unsigned char> buffer;
						buffer.resize(bufsize);

						if (!file.read((char*)&buffer[0], bufsize))
						{
							std::cerr << "DDS loader: An error occured while reading DDS data." << std::endl;
							file.close();
							return tex;
						}
						else
						{
							data.push_back(buffer);
						}

						curw = (curw > 1 ? curw / 2 : 1);
						curh = (curh > 1 ? curh / 2 : 1);
					}
					file.close();
					std::vector<std::vector<std::vector<unsigned char>>> _data;
					_data.push_back(data);
					tex.setData(_data);
					tex.setType(type);
					tex.setFormat(format);
					tex.setSize(_header.dwWidth, _header.dwHeight);
					return tex;
				}
				else if(_header.dwMipMapCount == 0) //no mipmaps
				{
					unsigned long bufsize = _header.dwPitchOrLinearSize;
					std::vector<unsigned char> buffer;
					buffer.resize(bufsize);
					if (!file.read((char*)&buffer[0], bufsize))
					{
						std::cerr << "DDS loader: An error occured while reading DDS data." << std::endl;
						file.close();
						return tex;
					}
					else
					{
						file.close();
						//process data
						std::vector<std::vector<std::vector<unsigned char>>> _data;
						_data.push_back(std::vector<std::vector<unsigned char>>());
						_data[0].push_back(buffer);
						tex.setData(_data);
						tex.setType(type);
						tex.setFormat(format);
						tex.setSize(_header.dwWidth, _header.dwHeight);
						return tex;
					}
				}
			}		
			else //uncompressed texture
			{
				file.close();
				std::cerr << "DDS loader: Uncompressed textures are not supported yet." << std::endl;
				return tex;
			}
			break;
		case TEX_CUBEMAP:
			file.close();
			std::cerr << "DDS loader: Cubemaps are not supported yet." << std::endl;
			return tex;
			break;
		case TEX_3D:
			file.close();
			std::cerr << "DDS loader: Volume textures are not supported yet." << std::endl;
			return tex;
			break;
		//Not supported for now:
		case TEX_ARRAY:
			file.close();
			std::cerr << "DDS loader: DDS DX10 extension is not supported yet." << std::endl;
			return tex;
			break;		
		case TEX_DX10_CUBEMAP:
			file.close();
			std::cerr << "DDS loader: DDS DX10 extension is not supported yet." << std::endl;
			return tex;
			break;
		default:
			file.close();
			std::cerr << "DDS loader: Wrong textype parameter." << std::endl;
			return tex;
			break;
		}		
	}
	else
	{
		std::cerr << "DDS loader: DDS file is corrupted." << std::endl;
		file.close();
		return tex;
	}
}

bool loadDDSTex(const std::string& _filepath, Texture& tex)
{
	//Texture tex;
	std::ifstream file(_filepath.c_str(), std::ifstream::binary);
	DDS_HEADER _header;
	DDS_HEADER_DXT10 _dx10ExtHeader;
	bool ddsHeaderFound = false;
	bool dx10ExtHeaderFound = false;
	TEXTYPE type;
	if (!extractDDSHeader(file, _header, _dx10ExtHeader, ddsHeaderFound, dx10ExtHeaderFound))
	{
		std::cerr << "DDS file is corrupted." << std::endl;
		file.close();
		return false;
	}

	if (ddsHeaderFound)
	{
		// default
		type = TEX_2D;

		// cubemap?
		if ((_header.dwCaps & (DDSCAPS_COMPLEX)) && (_header.dwCaps2 & DDSCAPS2_CUBEMAP))
			type = TEX_CUBEMAP;

		// volume?
		if ((_header.dwCaps & (DDSCAPS_COMPLEX)) && (_header.dwCaps2 & DDSCAPS2_VOLUME) && (_header.dwFlags & DDSD_DEPTH) && (_header.dwDepth > 0))
			type = TEX_3D;

		switch (type)
		{
		case TEX_2D:
			if ((_header.dwFlags & DDSD_LINEARSIZE) && (_header.ddspf.dwFlags & DDPF_FOURCC)) //compressed texture
			{
				//format
				unsigned int components = 0;
				DWORD format;
				switch (_header.ddspf.dwFourCC)
				{
				case FOURCC_DXT1:
					format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
					components = 3;
					break;
				case FOURCC_DXT3:
					format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
					components = 4;
					break;
				case FOURCC_DXT5:
					format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
					components = 4;
					break;
				default:
					file.close();
					std::cerr << "DDS loader: Only DXT1, DXT3 and DXT5 compressed textures are supported." << std::endl;
					return false;
					break;
				}

				if ((_header.dwFlags & DDSD_MIPMAPCOUNT) && (_header.dwCaps & DDSCAPS_COMPLEX)) //DDS contains mipmaps
				{
					unsigned long bufsize = 0;
					unsigned long curw = _header.dwWidth;
					unsigned long curh = _header.dwHeight;
					std::vector<std::vector<unsigned char>> data;
					for (int i = 0; i < _header.dwMipMapCount; i++)
					{
						bufsize = std::max(1UL, ((curw + 3) / 4)) * std::max(1UL, ((curh + 3) / 4)) * (_header.ddspf.dwFourCC == FOURCC_DXT1 ? 8 : 16);
						std::vector<unsigned char> buffer;
						buffer.resize(bufsize);

						if (!file.read((char*)&buffer[0], bufsize))
						{
							std::cerr << "DDS loader: An error occured while reading DDS data." << std::endl;
							file.close();
							return false;
						}
						else
						{
							data.push_back(buffer);
						}

						curw = (curw > 1 ? curw / 2 : 1);
						curh = (curh > 1 ? curh / 2 : 1);
					}
					file.close();
					std::vector<std::vector<std::vector<unsigned char>>> _data;
					_data.push_back(data);
					tex.setData(_data);
					tex.setType(type);
					tex.setFormat(format);
					tex.setSize(_header.dwWidth, _header.dwHeight);
					return true;
				}
				else if (_header.dwMipMapCount == 0) //no mipmaps
				{
					unsigned long bufsize = _header.dwPitchOrLinearSize;
					std::vector<unsigned char> buffer;
					buffer.resize(bufsize);
					if (!file.read((char*)&buffer[0], bufsize))
					{
						std::cerr << "DDS loader: An error occured while reading DDS data." << std::endl;
						file.close();
						return false;
					}
					else
					{
						file.close();
						//process data
						std::vector<std::vector<std::vector<unsigned char>>> _data;
						_data.push_back(std::vector<std::vector<unsigned char>>());
						_data[0].push_back(buffer);
						tex.setData(_data);
						tex.setType(type);
						tex.setFormat(format);
						tex.setSize(_header.dwWidth, _header.dwHeight);
						return true;
					}
				}
			}
			else //uncompressed texture
			{
				file.close();
				std::cerr << "DDS loader: Uncompressed textures are not supported yet." << std::endl;
				return false;
			}
			break;
		case TEX_CUBEMAP:
			file.close();
			std::cerr << "DDS loader: Cubemaps are not supported yet." << std::endl;
			return false;
			break;
		case TEX_3D:
			file.close();
			std::cerr << "DDS loader: Volume textures are not supported yet." << std::endl;
			return false;
			break;
			//Not supported for now:
		case TEX_ARRAY:
			file.close();
			std::cerr << "DDS loader: DDS DX10 extension is not supported yet." << std::endl;
			return false;
			break;
		case TEX_DX10_CUBEMAP:
			file.close();
			std::cerr << "DDS loader: DDS DX10 extension is not supported yet." << std::endl;
			return false;
			break;
		default:
			file.close();
			std::cerr << "DDS loader: Wrong textype parameter." << std::endl;
			return false;
			break;
		}
	}
	else
	{
		std::cerr << "DDS loader: DDS file is corrupted." << std::endl;
		file.close();
		return false;
	}
}