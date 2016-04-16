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

GLuint loadDDSTexture2D(const std::string& _filepath)
{
	std::ifstream file(_filepath.c_str(), std::ifstream::binary);
	DDS_HEADER _header;
	DDS_HEADER_DXT10 _dx10ExtHeader;
	bool ddsHeaderFound = false;
	bool dx10ExtHeaderFound = false;
	if (!extractDDSHeader(file, _header, _dx10ExtHeader, ddsHeaderFound, dx10ExtHeaderFound))
	{
		file.close();
		return 0;
	}

	if (ddsHeaderFound)
	{
		//byte size of each level for compressed textures (DXT 1 - 5) := max(1, ( (width + 3) / 4 ) ) x max(1, ( (height + 3) / 4 ) ) x 8(DXT1) or 16(DXT2-5)
		if (dx10ExtHeaderFound)
		{

		}
		else
		{

		}
	}
	else
	{
		file.close();
		return 0;
	}
}