#include "DDSLoader.h"


DDSLoader::DDSLoader()
{
}


DDSLoader::~DDSLoader()
{
}

bool DDSLoader::extractDDSHeader(std::ifstream& _file, DDS_HEADER& _header, DDS_HEADER_DXT10& _dx10ExtHeader, bool& _foundDDSHeader, bool& _foundDX10ExtHeader)
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

bool DDSLoader::loadDDSTex(const std::string& _filepath, Texture& _tex)
{
	//Texture tex;
	std::ifstream file(_filepath.c_str(), std::ifstream::binary);
	DDS_HEADER _header;
	DDS_HEADER_DXT10 _dx10ExtHeader;
	bool ddsHeaderFound = false;
	bool dx10ExtHeaderFound = false;
	TEXTYPE type;
	if (!DDSLoader::extractDDSHeader(file, _header, _dx10ExtHeader, ddsHeaderFound, dx10ExtHeaderFound))
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
		{
			Texture2D* tex = dynamic_cast<Texture2D*>(&_tex);
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
					for (DWORD i = 0; i < _header.dwMipMapCount; i++)
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
							Image2D im(format, 0, 0, components, curw, curh, i, buffer);
							tex->addMipMap(im);
						}

						curw = (curw > 1 ? curw / 2 : 1);
						curh = (curh > 1 ? curh / 2 : 1);
					}
					file.close();
					tex->setWidth(_header.dwWidth);
					tex->setHeight(_header.dwHeight);
					tex->setGLInternalFormat(format);
					tex->setGLFormat(0);
					tex->setGLType(0);
					tex->setBindingOptions(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
					
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
						Image2D im(format, 0, 0, components, _header.dwWidth, _header.dwHeight, 0, buffer);
						tex->addMipMap(im);
						tex->setWidth(_header.dwWidth);
						tex->setHeight(_header.dwHeight);
						tex->setGLInternalFormat(format);
						tex->setGLFormat(0);
						tex->setGLType(0);
						tex->setBindingOptions(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						return true;
					}
				}
			}
			else //uncompressed texture
			{
				if ((_header.dwFlags & (DDPF_RGB | DDPF_ALPHA)) && (_header.ddspf.dwRGBBitCount == 32)) //uncompressed R8G8B8A8 data?
				{
					if ((_header.dwFlags & DDSD_MIPMAPCOUNT) && (_header.dwCaps & DDSCAPS_COMPLEX)) //DDS contains mipmaps
					{
						unsigned long bufsize = 0;
						unsigned long curw = _header.dwWidth;
						unsigned long curh = _header.dwHeight;
						DWORD format = GL_RGBA8;		//RGBA32 unsigned integer data

						for (DWORD i = 0; i < _header.dwMipMapCount; i++)
						{
							bufsize = ((curw * 32 + 7) / 8) * curh;
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
								Image2D im(format, GL_RGBA, GL_UNSIGNED_BYTE, 4, curw, curh, i, buffer);
								tex->addMipMap(im);
							}

							curw = (curw > 1 ? curw / 2 : 1);
							curh = (curh > 1 ? curh / 2 : 1);
						}
						file.close();
						tex->setWidth(_header.dwWidth);
						tex->setHeight(_header.dwHeight);
						tex->setGLInternalFormat(format);
						tex->setGLFormat(GL_RGBA);
						tex->setGLType(GL_UNSIGNED_BYTE);
						tex->setBindingOptions(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						return true;
					}
					else if (_header.dwMipMapCount == 0) //no mipmaps
					{
						unsigned long bufsize = ((_header.dwWidth * 32 + 7) / 8) * _header.dwHeight;
						DWORD format = GL_RGBA8;
						std::vector<unsigned char> buffer;
						buffer.resize(bufsize);
						if (!file.read((char*)&buffer[0], bufsize))
						{
							std::cerr << "DDS loader: An error occured while reading DDS data." << std::endl;
							file.close();
							//delete tex;
							return false;
						}
						else
						{
							file.close();
							//process data
							Image2D im(format, GL_RGBA, GL_UNSIGNED_BYTE, 4, _header.dwWidth, _header.dwHeight, 0, buffer);
							tex->addMipMap(im);
							tex->setWidth(_header.dwWidth);
							tex->setHeight(_header.dwHeight);
							tex->setGLInternalFormat(format);
							tex->setGLFormat(GL_RGBA);
							tex->setGLType(GL_UNSIGNED_BYTE);
							tex->setBindingOptions(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
							return true;
						}
					}
				}
				else
				{
					file.close();
					std::cerr << "DDS loader: The format is not supported yet." << std::endl;
					//delete tex;
					return false;
				}
			}
			break;
		}
		case TEX_CUBEMAP:
		{
			file.close();
			std::cerr << "DDS loader: Cubemaps are not supported yet." << std::endl;
			return false;
			break;
		}
		case TEX_3D:
		{
			file.close();
			std::cerr << "DDS loader: Volume textures are not supported yet." << std::endl;
			return false;
			break;
			//Not supported for now:
		}
		case TEX_ARRAY:
		{
			file.close();
			std::cerr << "DDS loader: DDS DX10 extension is not supported yet." << std::endl;
			return false;
			break;
		}
		case TEX_DX10_CUBEMAP:
		{
			file.close();
			std::cerr << "DDS loader: DDS DX10 extension is not supported yet." << std::endl;
			return false;
			break;
		}
		default:
		{
			file.close();
			std::cerr << "DDS loader: Wrong textype parameter." << std::endl;
			return false;
			break;
		}
		}
	}
	else
	{
		std::cerr << "DDS loader: DDS file is corrupted." << std::endl;
		file.close();
		return false;
	}
	return false;
}

Texture* DDSLoader::loadDDSTex(const std::string& _filepath)
{
	
	std::ifstream file(_filepath.c_str(), std::ifstream::binary);
	DDS_HEADER _header;
	DDS_HEADER_DXT10 _dx10ExtHeader;
	bool ddsHeaderFound = false;
	bool dx10ExtHeaderFound = false;
	TEXTYPE type;
	if (!DDSLoader::extractDDSHeader(file, _header, _dx10ExtHeader, ddsHeaderFound, dx10ExtHeaderFound))
	{
		std::cerr << "DDS file is corrupted." << std::endl;
		file.close();
		return 0;
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
		{
			Texture2D* tex = new Texture2D("");
			tex->setPath(_filepath);
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
					delete tex;
					return 0;
					break;
				}

				if ((_header.dwFlags & DDSD_MIPMAPCOUNT) && (_header.dwCaps & DDSCAPS_COMPLEX)) //DDS contains mipmaps
				{
					unsigned long bufsize = 0;
					unsigned long curw = _header.dwWidth;
					unsigned long curh = _header.dwHeight;
					tex->setWidth(curw);
					tex->setHeight(curh);
					for (DWORD i = 0; i < _header.dwMipMapCount; i++)
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
							Image2D im(format, 0, 0, components, curw, curh, i, buffer);
							tex->addMipMap(im);
						}

						curw = (curw > 1 ? curw / 2 : 1);
						curh = (curh > 1 ? curh / 2 : 1);
					}
					file.close();
					tex->setGLInternalFormat(format);
					tex->setGLFormat(0);
					tex->setGLType(0);
					tex->setBindingOptions(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
					return tex;
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
						delete tex;
						return 0;
					}
					else
					{
						file.close();
						//process data
						Image2D im(format, 0, 0, components, _header.dwWidth, _header.dwHeight, 0, buffer);
						tex->addMipMap(im);
						tex->setGLInternalFormat(format);
						tex->setGLFormat(0);
						tex->setGLType(0);
						tex->setBindingOptions(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						return tex;
					}
				}
			}
			else //uncompressed texture
			{
				if ((_header.ddspf.dwFlags & (DDPF_RGB | DDPF_ALPHA)) && (_header.ddspf.dwRGBBitCount == 32)) //uncompressed R8G8B8A8 data?
				{
					if ((_header.dwFlags & DDSD_MIPMAPCOUNT) && (_header.dwCaps & DDSCAPS_COMPLEX)) //DDS contains mipmaps
					{
						unsigned long bufsize = 0;
						unsigned long curw = _header.dwWidth;
						unsigned long curh = _header.dwHeight;
						DWORD format = GL_RGBA8;		//RGBA32 unsigned integer data

						std::vector<Image2D> data;
						for (DWORD i = 0; i < _header.dwMipMapCount; i++)
						{
							bufsize = ((curw * 32 + 7) / 8) * curh;
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
								Image2D im(format, GL_RGBA, GL_UNSIGNED_BYTE, 4, curw, curh, i, buffer);
								tex->addMipMap(im);
							}

							curw = (curw > 1 ? curw / 2 : 1);
							curh = (curh > 1 ? curh / 2 : 1);
						}
						file.close();
						tex->setGLInternalFormat(format);
						tex->setGLFormat(GL_RGBA);
						tex->setGLType(GL_UNSIGNED_BYTE);
						tex->setBindingOptions(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
						return tex;
					}
					else if (_header.dwMipMapCount == 0) //no mipmaps
					{
						unsigned long bufsize = ((_header.dwWidth * 32 + 7) / 8) * _header.dwHeight;
						DWORD format = GL_RGBA8;
						std::vector<unsigned char> buffer;
						buffer.resize(bufsize);
						if (!file.read((char*)&buffer[0], bufsize))
						{
							std::cerr << "DDS loader: An error occured while reading DDS data." << std::endl;
							file.close();
							delete tex;
							return 0;
						}
						else
						{
							file.close();
							//process data
							Image2D im(format, GL_RGBA, GL_UNSIGNED_BYTE, 4, _header.dwWidth, _header.dwHeight, 0, buffer);
							tex->addMipMap(im);
							tex->setGLInternalFormat(format);
							tex->setGLFormat(GL_RGBA);
							tex->setGLType(GL_UNSIGNED_BYTE);
							tex->setBindingOptions(GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
							return tex;
						}
					}
				}
				else
				{
					file.close();
					std::cerr << "DDS loader: The format is not supported yet." << std::endl;
					delete tex;
					return 0;
				}
			}
			break;
		}
		case TEX_CUBEMAP:
		{
			file.close();
			std::cerr << "DDS loader: Cubemaps are not supported yet." << std::endl;
			return 0;
			break;
		}
		case TEX_3D:
		{
			file.close();
			std::cerr << "DDS loader: Volume textures are not supported yet." << std::endl;
			return 0;
			break;
			//Not supported for now:
		}
		case TEX_ARRAY:
		{
			file.close();
			std::cerr << "DDS loader: DDS DX10 extension is not supported yet." << std::endl;
			return 0;
			break;
		}
		case TEX_DX10_CUBEMAP:
		{
			file.close();
			std::cerr << "DDS loader: DDS DX10 extension is not supported yet." << std::endl;
			return 0;
			break;
		}
		default:
		{
			file.close();
			std::cerr << "DDS loader: Wrong textype parameter." << std::endl;
			return 0;
			break;
		}
		}
	}
	else
	{
		std::cerr << "DDS loader: DDS file is corrupted." << std::endl;
		file.close();
		return 0;
	}
	return 0;
}
